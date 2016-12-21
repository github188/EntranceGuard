#include "equipment.h"
#include <QFile>
#include "../myhelper.h"
#include "../AppConfig/myapp.h"
quint32 Equipment::readDelayTime=10*1000;
quint16 Equipment::tcpPort=myApp::TcpPort.toInt();
quint16 Equipment::udpPort=myApp::UdpPort.toInt();
quint64 Equipment::detectDisconnectTime=5*1000;//超过5秒没收到报读取时间报文代表掉线
quint64 Equipment::reConnectTime=10*1000;//掉线后10秒重连
quint64 Equipment::Addsecond=1*1000;//秒钟加1
extern QString sFilePath;
void delaySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void delaySleep1(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void delaySleep2(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
/*
*用十六进制表示十进制数 比如18改为0x18
 */
static quint8 decimalToHexDisplay(quint8 dec)
{
    static quint8 hex=0x00;
    hex = dec/10;
    hex = hex<<4;
    hex = hex|(dec%10);
    return hex;

}
Equipment::Equipment(QObject *parent) : QObject(parent)
{
    autoLink=1;
    indexInTree=0;
    parentName="";
    parentIndexInTree=0;
    equParaData.slaveStatus.connectStatus=0;
    readTimeTimer = new QTimer(this);
    reConnectTimer = new QTimer(this);
    detectDisconnectTimer = new QTimer(this);
    slaveTimeSet = new QTimer(this);
    //初始化缓冲区 1K
    comBuffer = new CCycleBuffer(1024*100);
    sendDataBuffer = new CCycleBuffer(1024*100);
    equNameMac="";
    //是否选中状态 默认为选中
    selectStatus=0;
    controlFlag=0;
    useModule=0;
    usemoduleModel=0;
    tcpClient=NULL;
    connect(this,SIGNAL(sigAnalysisData()),this,SLOT(slotAnalysisData()));

    connect(this,SIGNAL(sigSendRequest()),this,SLOT(slotSendRequest()));
    connect(reConnectTimer, SIGNAL(timeout()), this, SLOT(reConnect()));
    connect(detectDisconnectTimer, SIGNAL(timeout()), this, SLOT(connectionClosedByServer()));
    connect(readTimeTimer, SIGNAL(timeout()), this, SLOT(slotReadSlaveTime()));
    connect(slaveTimeSet,SIGNAL(timeout()),this,SLOT(slotAutoAddTimeSec()));
}
Equipment::~Equipment()
{
    delete comBuffer;
    delete sendDataBuffer;
    if(tcpClient == NULL)
    {
        return;
    }
    //连网方式 0－服务器 1-客户端
    switch(equParaData.net.networkModel)
    {
    case 0://服务器模式
        tcpClient->disconnectFromHost();
        tcpClient->close();
        tcpClient->deleteLater();
        delete tcpClient;
        break;
    case 1://客户端模式
        //tcpClient->close();
        break;
    default:
        break;
    }


}
void Equipment::setEquQTcpSocket(QTcpSocket *client)
{
    tcpClient = client;
}
void Equipment::EquInit(paraData par)
{
    equParaData=par;
    //连网方式 0－服务器 1-客户端
    switch(equParaData.net.networkModel)
    {
    case 0:
        tcpClient = new QTcpSocket(this);
        break;
    case 1:
        break;
    default:
        break;
    }
    //连接信号
    if(tcpClient != NULL)
    {
        connect(tcpClient,SIGNAL(connected()),this,SLOT(connectedToServer()));
        connect(tcpClient,SIGNAL(disconnected()),this,SLOT(connectionClosedByServer()));
        connect(tcpClient,SIGNAL(readyRead()),this,SLOT(receiveData()));
        connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error()));


        //连网方式 0－服务器 1-客户端
        switch(equParaData.net.networkModel)
        {
        case 0:
            connectToServer();
            break;
        case 1:
            connectedToServer();
            break;
        default:
            break;
        }
        if(selectStatus)
        {
            emit sigDataUpdateToForm(this);
        }
    }
}
void Equipment::emitRefreshFormAndStatusChangedSignals()
{
    emit sigStatusChanged();
    if(selectStatus)
    {
        emit sigDataUpdateToForm(this);
    }
}
void Equipment::connectToServer()
{
    QHostAddress address(equParaData.net.ip);
    if(tcpClient)
    {
        tcpClient->disconnectFromHost();
        //tcpClient->waitForDisconnected();
        tcpClient->close();
        tcpClient->connectToHost(address,tcpPort);
    }
    equParaData.slaveStatus.connectStatus=2;
    /*
    if(selectStatus)
    {
        emit sigConnecting();
    }*/
    emitRefreshFormAndStatusChangedSignals();
    QLOG_INFO()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"tcp connecting...";
    //tcpClient->waitForConnected(3000);
}
void Equipment::connectedToServer()
{
    //tcpClient->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    reConnectTimer->stop();
    QLOG_INFO()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"tcp connected";
    equParaData.slaveStatus.connectStatus=1;
    /*
    if(selectStatus)
    {
        emit sigConnected();
    }*/

    //读取设备参数
    delaySleep1(20);
    slotreadSlavePara();
    //读取设备状态
    delaySleep1(30);
    slotReadSlaveStatus();
    //获取设备工作参数
    delaySleep1(30);
    slotGetWorkPara();
    //读取音量
    delaySleep1(30);
    slotReadHyVolume();
    //获取照明灯和广告灯参数
    delaySleep1(30);
    slotGetLineAndAdLinePara();
    //读取所有声音的md5;
    delaySleep1(30);
    slotReadAllSoundMd5();
    //读取LED内容;
    for(int i=0;i<3;i++)
    {
        delaySleep1(30);
        slotReadLed(i);
    }
    delaySleep1(100);
    if(equParaData.getTwoMainVer() > 27||(equParaData.slaveType == 1))
    {
        //如果是2.8版本则读取报警参数和报警状态
        delaySleep1(30);
        slotGetAlarmPara();
        delaySleep1(30);
        slotGetAlarmStatus();
    }
    delaySleep1(30);
    //读取时间
    slotReadSlaveTime();
    //启动定时读取时间
    readTimeTimer->start(readDelayTime);
    //slaveTimeSet->start(Addsecond);
    //启动断线检测
    //detectDisconnectTimer->start(detectDisconnectTime);

    emitRefreshFormAndStatusChangedSignals();
    //emit sigStatusChanged();

}
static quint8 convertTcpFuncToSendDataCount(TcpFuncCode function)
{
    static quint8 dataCount=0;//值等于总字节数－1
    dataCount=0;
    switch(function)
    {
    case func_tcp_read_slave_para:
        dataCount=3;
        break;
    case func_tcp_read_slave_time:
        dataCount=3;
        break;
    case func_tcp_set_slave_time:
        dataCount=15;
        break;
    case func_tcp_read_slave_status:
        dataCount=3;
        break;
    case func_tcp_read_work_para:
        dataCount=3;
        break;
    case func_tcp_set_work_para:
        dataCount=15;
        break;
    case func_tcp_read_lamp_Para:
        dataCount=3;
        break;
    case func_tcp_set_lineAndAdLine_para:
        dataCount=11;
        break;
    case func_tcp_openDoor:
        dataCount=3;
        break;
    case func_tcp_closeLock:
        dataCount=3;
        break;
    case func_tcp_read_msgCount:
        dataCount=3;
        break;
    case func_tcp_read_msg:
        dataCount=3;
        break;
    case func_tcp_read_led:
        dataCount=3;
        break;
    case func_tcp_EnableOrDisable_slave:
        dataCount=3;
        break;
    case func_tcp_read_alarmStatus:
    case func_tcp_read_alarmPara:
    case func_tcp_clear_alarm:
    case func_tcp_set_status:
    case func_tcp_read_key:
        dataCount=3;
        break;
    case func_tcp_set_alarmPara:
        dataCount=15;
        break;
    case func_tcp_read_hy_volume:
        dataCount=3;
        break;
    case func_tcp_set_hy_volume:
    case func_tcp_set_syz_volume://设置使用中语音音量
    case func_tcp_set_whz_volume://设置维护中语音音量
    case func_tcp_set_timeoutWarn_volume://设置超时提醒语音音量
    case func_tcp_set_timeoutTiShi_volume://设置超时提示语音音量
    case func_tcp_set_outDoor_volume://设置出门语音音量大小
    case func_tcp_set_openDoor_volume://设置门开提示语音音量大小
    case func_tcp_set_failLock_volume://设置门未锁好语音音量大小
    case func_tcp_set_laDoor_volume://设置拉门语音音量大小
    case func_tcp_read_hy_md5:
    case func_tcp_read_syz_md5:
    case func_tcp_read_whz_md5:
    case func_tcp_read_timeoutWarn_md5:
    case func_tcp_read_timeoutTiShi_md5:
    case func_tcp_read_outDoor_md5:
    case func_tcp_read_openDoor_md5:
    case func_tcp_read_failLock_md5:
    case func_tcp_read_laDoor_md5:
        dataCount=3;
        break;
    case func_tcp_read_hy_sound:
    case func_tcp_read_syz_sound:
    case func_tcp_read_whz_sound:
    case func_tcp_read_timeoutWarn_sound:
    case func_tcp_read_timeoutTiShi_sound:
    case func_tcp_read_outDoor_sound:
    case func_tcp_read_openDoor_sound:
    case func_tcp_read_failLock_sound:
    case func_tcp_read_laDoor_sound:
        dataCount=3;
        break;
    case func_tcp_set_hy_md5:
    case func_tcp_set_syz_md5:
    case func_tcp_set_whz_md5:
    case func_tcp_set_timeoutWarn_md5:
    case func_tcp_set_timeoutTiShi_md5:
    case func_tcp_set_outDoor_md5:
    case func_tcp_set_openDoor_md5:
    case func_tcp_set_failLock_md5:
    case func_tcp_set_laDoor_md5:
        dataCount=19;
        break;
    case func_tcp_play_sound:
        dataCount=3;
        break;
    case func_tcp_delete_key:
        dataCount = 9;
        break;
    case func_tcp_write_mainKey:
        dataCount = 18;
        break;
    default:
        dataCount=0;//说明头字节出现错误
        break;
    }
    return dataCount;
}
void Equipment::slotSendRequest()
{
    static QByteArray block;
    static quint64 dataCount=0;
    static quint8 startBuf=0;
    static TcpFuncCode func=func_tcp_null;
    static quint8 model=0;
    static quint64 readedCount=0;
    static char buf[1000];
    delaySleep1(20);
    if (!sendDataBuffer->isEmpty())
    {
        if((1!=equParaData.slaveStatus.connectStatus))
        {
            return;
        }
        //计算需要读取的字节数量 每次发送一组报文

        sendDataBuffer->read((char*)&buf[0],1);
        func=(TcpFuncCode)((quint8)buf[0]);
        if(func == func_tcp_set_led)
        {
            sendDataBuffer->read((char*)&buf[1],2);
            dataCount = (quint8)buf[2]+1;
            startBuf=3;
        }
        else if(func == func_tcp_set_hy_sound ||
                func == func_tcp_set_syz_sound ||
                func == func_tcp_set_whz_sound ||
                func == func_tcp_set_timeoutWarn_sound ||
                func == func_tcp_set_timeoutTiShi_sound ||
                func == func_tcp_set_outDoor_sound ||
                func == func_tcp_set_openDoor_sound ||
                func == func_tcp_set_failLock_sound ||
                func == func_tcp_set_laDoor_sound)
        {
            sendDataBuffer->read((char*)&buf[1],1);
            model = (quint8)buf[1];
            switch(model)
            {
            case 0:
            {
                dataCount = 8;
                startBuf=2;
                break;
            }
            case 1:
            {
                sendDataBuffer->read((char*)&buf[2],6);

                quint16 sum = (quint8)buf[3];
                sum = (sum<<8) | (quint8)buf[2];

                dataCount = sum+4;
                startBuf=8;
                break;
            }
            case 2:
            {
                dataCount = 8;
                startBuf=2;
                break;
            }
            default:
                break;
            }

        }
        else
        {
            dataCount = convertTcpFuncToSendDataCount(func);
            startBuf=1;
        }

        readedCount = sendDataBuffer->read((char*)&buf[startBuf],dataCount);

        block=QByteArray(buf,dataCount+startBuf);

        //QLOG_INFO()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"send data:"<<block.toHex();
        if(1==equParaData.slaveStatus.connectStatus)
        {
            tcpClient->write(block);
            tcpClient->flush();//立即发送
            QLOG_INFO()<<"sendDataFrame:"<<block.toHex();
        }
        else
        {
            QLOG_ERROR()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"TCP disconnect，data send error";
        }
    }
    else//如果没有这个等待，就会有很多空循环，CPU会很高。
    {
        sendDataBuffer->waitNotEmpty();
    }


}
//获取设备名称
QString Equipment::GetEquName()
{
    return equParaData.name;
}
//获取设备名称
QString Equipment::GetEquNameMac()
{
    if(equNameMac == "")
    {
        equNameMac = equParaData.name+"("+equParaData.net.macAddr+")";
    }
    return equNameMac;
}
//获取设备名称
QString Equipment::GetEquVersion()
{
    return equParaData.version;
}
//获取设备名称
QString Equipment::GetEquMac()
{
    return equParaData.net.macAddr;
}
//获取设备类型
quint8 Equipment::GetSlaveType()
{
    return equParaData.slaveType;
}
//获取设备所有参数
paraData *Equipment::GetEquAll()
{
    return &(this->equParaData);
}
static quint8 convertTcpFuncToReceiveDataCount(TcpFuncCode function)
{
    static quint8 dataCount=0;//值等于总字节数－1
    dataCount=0;
    switch(function)
    {
    case func_tcp_read_slave_para:
        dataCount=63;
        break;
    case func_tcp_read_slave_time:
        dataCount=15;
        break;
    case func_tcp_set_slave_time:
        dataCount=3;
        break;
    case func_tcp_read_slave_status:
        dataCount=15;
        break;
    case func_tcp_sd_slave_fangQieWarning:
        dataCount=3;
        break;
    case func_tcp_sd_slave_buttonWarning:
        dataCount=3;
        break;
    case func_tcp_sd_slave_temperature:
        dataCount=7;
        break;
    case func_tcp_sd_slave_status:
        dataCount=7;
        break;
    case func_tcp_read_work_para:
        dataCount=15;
        break;
    case func_tcp_set_work_para:
        dataCount=3;
        break;
    case func_tcp_read_lamp_Para:
        dataCount=11;
        break;
    case func_tcp_set_lineAndAdLine_para:
        dataCount=3;
        break;
    case func_tcp_openDoor:
        dataCount=3;
        break;
    case func_tcp_closeLock:
        dataCount=3;
        break;
    case func_tcp_read_msgCount:
        dataCount=7;
        break;
    case func_tcp_read_msg:
        dataCount=3;
        break;
    case func_tcp_read_alarmStatus:
    case func_tcp_read_alarmPara:
    case func_tcp_sd_slave_alarmStatus:
        dataCount=15;
        break;
    case func_tcp_set_led:
    case func_tcp_set_alarmPara:
    case func_tcp_clear_alarm:
    case func_tcp_EnableOrDisable_slave:
    case func_tcp_set_status:
    case func_tcp_delete_key:
    case func_tcp_write_mainKey:
        dataCount=3;
        break;
    case func_tcp_read_hy_volume:
        dataCount=3;
        break;
    case func_tcp_set_hy_volume:
    case func_tcp_set_syz_volume://设置使用中语音音量
    case func_tcp_set_whz_volume://设置维护中语音音量
    case func_tcp_set_timeoutWarn_volume://设置超时提醒语音音量
    case func_tcp_set_timeoutTiShi_volume://设置超时提示语音音量
    case func_tcp_set_outDoor_volume://设置出门语音音量大小
    case func_tcp_set_openDoor_volume://设置门开提示语音音量大小
    case func_tcp_set_failLock_volume://设置门未锁好语音音量大小
    case func_tcp_set_laDoor_volume://设置拉门语音音量大小
        dataCount=3;
        break;
    case func_tcp_read_hy_md5://
    case func_tcp_read_syz_md5://
    case func_tcp_read_whz_md5://
    case func_tcp_read_timeoutWarn_md5://
    case func_tcp_read_timeoutTiShi_md5://
    case func_tcp_read_outDoor_md5://
    case func_tcp_read_openDoor_md5:
    case func_tcp_read_failLock_md5:
    case func_tcp_read_laDoor_md5:
        dataCount=19;
        break;
    case func_tcp_set_hy_md5://
    case func_tcp_set_syz_md5://
    case func_tcp_set_whz_md5://
    case func_tcp_set_timeoutWarn_md5://
    case func_tcp_set_timeoutTiShi_md5://
    case func_tcp_set_outDoor_md5://
    case func_tcp_set_openDoor_md5:
    case func_tcp_set_failLock_md5:
    case func_tcp_set_laDoor_md5:
        dataCount=3;
        break;
    case func_tcp_set_hy_sound://
    case func_tcp_set_syz_sound://
    case func_tcp_set_whz_sound://
    case func_tcp_set_timeoutWarn_sound://
    case func_tcp_set_timeoutTiShi_sound://
    case func_tcp_set_outDoor_sound://
    case func_tcp_set_openDoor_sound:
    case func_tcp_set_failLock_sound:
    case func_tcp_set_laDoor_sound:
        dataCount=3;
        break;
    case func_tcp_play_sound:
        dataCount=3;
        break;
    case func_tcp_sd_addkey_value:
        dataCount=17;
        break;
    case func_tcp_sd_inkey_value:
        dataCount=21;
        break;
    default:
        dataCount=0;//说明头字节出现错误
        break;

    }
    return dataCount;
}
//分析接收的数据
void Equipment::slotAnalysisData()
{
    //static bool flag=false;
    static quint8 startBuf=0;
    static TcpFuncCode func=func_tcp_null;
    static char dbuf[1000];//读取控制器记录最多230个字节
    static quint8 ret=1;
    while(!comBuffer->isEmpty())
    {
        if (!comBuffer->isEmpty())
        {
            //处理完一帧数据后func置0 重新读取功能码
            if(func == 0)
            {
                comBuffer->read((char*)&dbuf[0], 1);
                func=(TcpFuncCode)((quint8)dbuf[0]-KEY);

                //QLOG_INFO()<<"func:"<<func;

                //计算需要读取的数据的个数
                if(func == func_tcp_read_msg)
                {
                    comBuffer->read((char*)&dbuf[1], 1);
                    blockSize = (quint8)dbuf[1];
                    startBuf = 2;
                }
                else if(func ==func_tcp_read_led)
                {
                    comBuffer->read((char*)&dbuf[1], 3);
                    blockSize = (quint8)dbuf[3]+1;
                    startBuf = 4;
                }
                else if((func == func_tcp_read_hy_sound)||(func == func_tcp_read_syz_sound)
                        ||(func == func_tcp_read_whz_sound)||(func == func_tcp_read_timeoutWarn_sound)
                        ||(func == func_tcp_read_timeoutTiShi_sound)||(func == func_tcp_read_outDoor_sound)
                        ||(func == func_tcp_read_openDoor_sound)||(func == func_tcp_read_failLock_sound)
                        ||(func == func_tcp_read_laDoor_sound))
                {
                    comBuffer->read((char*)&dbuf[1], 6);
                    quint8 sizeH = (quint8)dbuf[6];
                    quint8 sizeL = (quint8)dbuf[5];
                    blockSize = sizeH;
                    blockSize = ((blockSize<<8)|sizeL)+5;
                    startBuf = 7;
                }
                else if(func == func_tcp_read_key)
                {
                    comBuffer->read((char*)&dbuf[1], 2);
                    blockSize = (quint16)((dbuf[2]<<8)|(dbuf[1]))*16 + 1;
                    startBuf = 3;
                }
                else
                {
                    blockSize = convertTcpFuncToReceiveDataCount(func);
                    startBuf = 1;
                }
            }
            //QLOG_INFO()<<"blockSize:"<<blockSize;
            //QLOG_INFO()<<"startBuf:"<<startBuf;
            if(comBuffer->getFreeSize()>=blockSize)
            {

                //读取剩余数据并处理
                comBuffer->read((char*)&dbuf[startBuf],blockSize);
                for(i=0;i<funcTcpCountNum;i++)
                {
                    if(func==funcTcpArray[i].ucFunctionCode)
                    {
                        ret=funcTcpArray[i].pxHandler(dbuf,&this->equParaData);
                        break;//结束循环
                    }
                }
                if(selectStatus)
                {
                    //用读取时间返回数据作为心跳包
                    switch(func)
                    {
                    case func_tcp_read_slave_para:
                        emit sigUpdateTSlave(this);
                        emit sigRefreshSlavePara(this->GetEquAll());
                        break;
                    case func_tcp_read_slave_time:
                        //停止断线检测定时器
                        detectDisconnectTimer->stop();
                        //启动时间
                        slaveTimeSet->start(Addsecond);

                        emit sigRefreshTime(this->GetEquAll());
                        break;
                    case func_tcp_openDoor:
                        //发送开门完成信号
                        emit sigOpenDoorDone(ret);
                        break;
                    case func_tcp_closeLock:
                        //发送闭锁\开锁完成信号和前一个状态值
                        emit sigCloseLockRet(ret,this->equParaData.slaveStatus.lockStatus);
                        break;
                    case func_tcp_read_work_para:
                        //更新数据库
                        emit sigRefreshFangHuCangPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_read_lamp_Para:
                        //更新数据库
                        emit sigRefreshZhaoMingPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_sd_slave_status:
                    {
                        //更新显示
                        emitRefreshFormAndStatusChangedSignals();
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                    }
                        break;
                    case func_tcp_read_alarmStatus:
                        emit sigRefreshStatus(this->GetEquAll());
                        break;
                    case func_tcp_sd_slave_alarmStatus:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        emit sigSlaveWarm(this);
                        break;
                    case func_tcp_read_alarmPara:
                        emit sigRefreshAlarmPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_sd_slave_fangQieWarning:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigSlaveWarm(this);
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_buttonWarning:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigSlaveWarm(this);
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_temperature:
                        emit sigRefreshStatus(this->GetEquAll());
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_read_msgCount:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_msg:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_led:
                        //更新数据库
                        //QLOG_INFO()<<"read led text";
                        emit sigRefreshLedPara(this->GetEquAll());
                        emit sigDataUpdateToDataBase(this);
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_set_hy_volume://
                        emit sigRefreshVoicePara(this->GetEquAll());
                        break;
                    case func_tcp_read_hy_volume:
                        emit sigRefreshVoicePara(this->GetEquAll());
                        if(useModule)
                        {useModule = 0;}
                        QLOG_INFO()<<"read sound volume end";
                        break;
                    case func_tcp_read_hy_md5://

                    case func_tcp_read_syz_md5://

                    case func_tcp_read_whz_md5://

                    case func_tcp_read_timeoutWarn_md5://

                    case func_tcp_read_timeoutTiShi_md5://

                    case func_tcp_read_outDoor_md5://

                    case func_tcp_read_openDoor_md5:

                    case func_tcp_read_failLock_md5:

                    case func_tcp_read_laDoor_md5:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        emit sigRefreshVoicePara(this->GetEquAll());
                        if(useModule)
                        {useModule = 0;}
                        break;
                        break;
                    case func_tcp_read_hy_sound://

                    case func_tcp_read_syz_sound://

                    case func_tcp_read_whz_sound://

                    case func_tcp_read_timeoutWarn_sound://

                    case func_tcp_read_timeoutTiShi_sound://

                    case func_tcp_read_outDoor_sound://

                    case func_tcp_read_openDoor_sound:

                    case func_tcp_read_failLock_sound:

                    case func_tcp_read_laDoor_sound:
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_set_lineAndAdLine_para:

                        break;
                    case func_tcp_set_led:
                        QLOG_INFO()<<"set led text return";
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_set_alarmPara:
                        QLOG_INFO()<<"set alarmPara return";
                        if(useModule)
                        {useModule = 0;}
                        break;
                    case func_tcp_read_key:
                        equParaData.readedKey=0;
                        break;
                    case func_tcp_sd_addkey_value:
                        QLOG_INFO()<<"slave add Key";
                        sigAddKey(equParaData.keyList.last());
                        break;
                    case func_tcp_sd_inkey_value:
                        //QLOG_INFO()<<"弹出有人进门窗口";
                        //emit sigShowIndoorWindow();
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    switch(func)
                    {
                    case func_tcp_read_slave_para:
                        emit sigUpdateTSlave(this);
                        break;
                    case func_tcp_read_slave_time:
                        //停止断线检测定时器
                        detectDisconnectTimer->stop();
                        //启动时间
                        slaveTimeSet->start(Addsecond);
                        break;
                    case func_tcp_read_work_para:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_read_lamp_Para:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_sd_slave_status:
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_alarmStatus:
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_sd_slave_temperature:
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    case func_tcp_read_msgCount:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_msg:
                        this->equParaData.resetFlag();
                        break;
                    case func_tcp_read_led:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_set_hy_volume://

                    case func_tcp_read_hy_volume:

                    case func_tcp_read_hy_md5://

                    case func_tcp_read_syz_md5://

                    case func_tcp_read_whz_md5://

                    case func_tcp_read_timeoutWarn_md5://

                    case func_tcp_read_timeoutTiShi_md5://

                    case func_tcp_read_outDoor_md5://

                    case func_tcp_read_openDoor_md5:

                    case func_tcp_read_failLock_md5:

                    case func_tcp_read_laDoor_md5:
                        //更新数据库
                        emit sigDataUpdateToDataBase(this);
                        break;
                    case func_tcp_sd_slave_buttonWarning:
                    case func_tcp_sd_slave_fangQieWarning:
                        emit sigSlaveWarm(this);
                        emit sigInsertFangHuCangLog(this->GetEquAll()->tempFangHuCangLogList);
                        break;
                    default:
                        break;
                    }
                }
                this->GetEquAll()->tempFangHuCangLogList.clear();
                func=func_tcp_null;//表示数据处理一次完毕
            }

        }
        else//如果没有这个等待，就会有很多空循环，CPU会很高。
        {
            comBuffer->waitNotEmpty();
        }
    }
}

//接收数据并写入缓冲区
void Equipment::receiveData()
{
    static QByteArray data=NULL;
    static quint64 dataLength=0;
    //QLOG_INFO()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"receive data and write buffer.";
    while(tcpClient->bytesAvailable()>0||(!data.isEmpty()))
    {
        //写入缓冲区
        if (!comBuffer->isFull())
        {
            if(data.isEmpty())
            {
                data = tcpClient->readAll();
            }
            dataLength = comBuffer->write(data.data(), data.length());
            if(dataLength!=data.length())
            {

                data.remove(0,dataLength);
                emit sigAnalysisData();
                QLOG_WARN()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"buffer is full，emit signal to read buf.";
                break;
            }
            else
            {
                data.clear();
                //QLOG_INFO()<<"Data written to complete. ";
            }
        }
        else//如果没有这个等待，会有很多空循环，CPU会很高
        {
            comBuffer->waitNotFull();
            emit sigAnalysisData();
            QLOG_WARN()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"buffer is full，emit signal to read buf.";
        }
    }
    //QLOG_INFO()<<"emit signal to read buf.";
    emit sigAnalysisData();

}
void Equipment::connectionClosedByServer()
{
    //readTimeTimer->stop();
    readTimeTimer->stop();//关闭定时读取时间（心跳）定时器
    detectDisconnectTimer->stop();//关闭断线检测定时器，否则会一直进入这个槽导致无法重新连接
    slaveTimeSet->stop();
    if(tcpClient)
    {
        if(tcpClient->isOpen())
        {
            tcpClient->close();//关闭连接
        }
    }
    equParaData.slaveStatus.connectStatus=0;
    /*
    if(selectStatus)
    {
        emit sigDisconnect();
    }
    */
    emitRefreshFormAndStatusChangedSignals();
    reConnectTimer->start(reConnectTime);//延时重新连接
    QLOG_INFO()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"TCP disconnected!.";
}
void Equipment::error()
{    
    tcpClient->close();//关闭连接
    tcpClient->disconnectFromHost();
    equParaData.slaveStatus.connectStatus=0;

    emitRefreshFormAndStatusChangedSignals();
    reConnectTimer->start(reConnectTime);
    QLOG_INFO()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"tcp connection error";
}
void Equipment::reConnect()
{
    if(equParaData.slaveStatus.connectStatus!=1)
    {
        reConnectTimer->stop();
        QLOG_INFO()<<"Mac:"<<this->equParaData.net.macAddr<<"--"<<"TCP reconnecting...";
        connectToServer();

    }
    else if(equParaData.slaveStatus.connectStatus==1)
    {
        reConnectTimer->stop();
    }
}
void Equipment::writeDataToBuffer(QByteArray *datagram)
{
    //将数据写入缓冲区并发送
    while(datagram->length())
    {
        //写入缓冲区
        if (!sendDataBuffer->isFull())
        {
            quint64 dataLength = sendDataBuffer->write(datagram->data(), datagram->length());
            if(dataLength!=datagram->length())
            {
                datagram->remove(0,dataLength);
                //如果缓冲区满 发送信号 发送数据
                emit sigSendRequest();
                break;
            }
            else
            {
                datagram->clear();
            }
        }
        else//如果没有这个等待，会有很多空循环，CPU会很高
        {
            sendDataBuffer->waitNotFull();
        }
    }
    //发送信号 发送数据
    emit sigSendRequest();
}
void Equipment::slotReadSlaveTime()
{
    slaveTimeSet->stop();
    //启动断线检测
    detectDisconnectTimer->start(detectDisconnectTime);
    //QLOG_INFO()<<"read time";
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_slave_time,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//读取设备参数
void Equipment::slotreadSlavePara()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_slave_para,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//读取设备状态
void Equipment::slotReadSlaveStatus()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_slave_status,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//设置时间
/*
 * year:年
   month:月
   day:日
   hour:时
   minutes:分
   second:秒
   week:星期
*/
void Equipment::slotSetSlaveTime(quint8 year, quint8 month, quint8 day,
                                 quint8 hour, quint8 minute, quint8 second,
                                 quint8 week)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=14;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    data[0]=decimalToHexDisplay(year);
    data[1]=decimalToHexDisplay(month);
    data[2]=decimalToHexDisplay(day);
    data[3]=decimalToHexDisplay(hour);
    data[4]=decimalToHexDisplay(minute);
    data[5]=decimalToHexDisplay(second);
    data[6]=decimalToHexDisplay(week);
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_slave_time,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//设置照明灯和广告灯参数
void Equipment::slotSetLineAndAdLinePara(paraZhaoMing *par)
{
    static QByteArray datagram=NULL;

    //需要发送的数据的个数
    static const quint16 dataNum=10;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    data[0]=par->Model;
    data[1]=decimalToHexDisplay(par->startHour);
    data[2]=decimalToHexDisplay(par->startMinute);
    data[3]=decimalToHexDisplay(par->endHour);
    data[4]=decimalToHexDisplay(par->endMinute);
    data[5]=par->manEnable;

    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_lineAndAdLine_para,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
    //设置完后再读取一次
    slotGetLineAndAdLinePara();
}

//获取照明灯和广告灯参数
void Equipment::slotGetLineAndAdLinePara()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_lamp_Para,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);

}
//设置工作参数
void Equipment::slotSetWorkPara(paraFangHuCang *par)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=14;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //公共参数
    data[0] = par->openLockTime;
    data[9] = par->lockModel;
    data[11] = par->fangQiewarnning;
    switch(equParaData.slaveType)
    {
    case 0://防护舱
        data[1] = par->OptBussinessTime;
        data[2] = par->timeOutRemind;
        data[3] = par->warnningDelayTime;
        data[4] = par->noManOpenLockTime;
        data[5] = 0;//备用
        data[6] = par->fanRunTemperature;
        data[7] = par->signalModel;
        data[8] = par->peopleEquModel;
        data[10] = par->kongCangLockorNot;
        break;
    case 1://加钞间
        data[1] = par->userNum;             //D2: 刷卡人数N
        data[2] = par->systemAlarmStatus;   //D3: 系统报警状态
        data[3] = par->setGuardDelayTime;   //D4: 设防延时时间
        data[4] = par->isMonitorOrNot;      //D5: 是否有人值守
        data[5] = par->inDoorModel;         //D6: 进门方式
        data[6] = par->outDoorModel;        //D7: 出门方式
        data[7] = 0;
        data[8] = 0;
        data[10] = 0;
        break;
    case 0xff:
        break;
    default:
        break;
    }



    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_work_para,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
    //设置完后再读取一次
    slotGetWorkPara();
}
//获取工作参数
void Equipment::slotGetWorkPara()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_work_para,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//启动\禁用控制器 0-禁用 1－启用
void Equipment::slotEanbleOrDisableSlave()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    if(this->GetEquAll()->slaveStatus.fanghucangStatus)
    {
        data[0] = 1;
    }
    else
    {
        data[0] = 0;
    }
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_EnableOrDisable_slave,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
    //QLOG_INFO()<<"发送信息："<<datagram.toHex();
}
//远程开门
void Equipment::slotRemoteOpenDoor()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_openDoor,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//远程闭锁
void Equipment::slotRemoteCloseLock()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    if(this->equParaData.slaveStatus.lockStatus)
        data[0] = 0;
    else
        data[0] = 1;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_closeLock,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//读取控制器内记录总条数
void Equipment::slotReadMsgCount()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_msgCount,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//读取控制器内记录总条数
//model:00—以一次读记录 01—接收成功，继续发送数据 03—读记录结束
//
void Equipment::slotReadMsg(quint8 preMsgCount, quint8 model)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint16 dataNum=2;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;

    data[0] = model;
    data[1] = preMsgCount;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_msg,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
void Equipment::slotReadSlaveMsg()
{
    static quint8 timeout=0;
    static quint8 delayTime=50;
    static quint8 TIMEOUT = 40;
    quint64 msgcount=0;
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //停止定时读取时间
    //readTimeTimer->stop();
    //停止断线检测
    //detectDisconnectTimer->stop();

    this->equParaData.setMsgCount(0);//已经获取的日志数量
    this->equParaData.cleariGetMsgCount();   //日志总数
    this->equParaData.iPreMsgCount=0;//上次获取的日志数量
    //第一步：先读记录总条数
    //45 00 00 04
    //设备返回记录总条数
    //验证后面读的记录条数是否完整
    this->equParaData.setFlag();
    slotReadMsgCount();
    timeout=0;
    while(this->equParaData.getFlag())
    {
        delaySleep1(delayTime);
        timeout++;
        if(timeout>=TIMEOUT)
        {
            timeout=0;
            //启动定时读取时间
            //readTimeTimer->start(readDelayTime);
            //启动断线检测
            //detectDisconnectTimer->start(detectDisconnectTime);
            QLOG_ERROR()<<"read slaveLog Number timeout!";
            return;
        }
        //qApp->processEvents();
    }
    msgcount=this->equParaData.GetMsgCount();
    if(msgcount<=0)
    {
        //启动定时读取时间
        //readTimeTimer->start(readDelayTime);
        //启动断线检测
        //detectDisconnectTimer->start(detectDisconnectTime);
        //更新日志
        FangHuCangLogItem * tempLog = new FangHuCangLogItem();
        tempLog->name = equParaData.name;
        tempLog->logItem = "读取记录完成，共读取"+QString::number(msgcount)+"条记录";
        equParaData.tempFangHuCangLogList.append(tempLog);
        emit sigInsertFangHuCangLog(equParaData.tempFangHuCangLogList);
        //清除列表
        equParaData.tempFangHuCangLogList.clear();

        QLOG_ERROR()<<"read slaveLog Number is error!";
        return;
    }
    emit sigSetWindowsTitle("读取记录");
    emit sigSoundSize(msgcount);

    QLOG_INFO()<<"read slaveLog Number is"+QString::number(msgcount);
    //第二步：发送第一次读记录命令
    //46 00 00 e0
    //设备返回1条记录
    this->equParaData.setFlag();
    slotReadMsg(0,0);
    timeout=0;
    while(this->equParaData.getFlag())
    {
        delaySleep1(delayTime);
        timeout++;
        if(timeout>=TIMEOUT)
        {
            timeout=0;
            //启动定时读取时间
            //readTimeTimer->start(readDelayTime);
            //启动断线检测
            //detectDisconnectTimer->start(detectDisconnectTime);
            QLOG_ERROR()<<"read slaveLog message timeout!";
            return;
        }
        //qApp->processEvents();
    }
    emit sigReadSoundSize(equParaData.iGetMsgCountCurrentValue());
    if(this->equParaData.iPreMsgCount <= 0)
    {
        //启动定时读取时间
        //readTimeTimer->start(readDelayTime);
        //启动断线检测
        //detectDisconnectTimer->start(detectDisconnectTime);
        QLOG_ERROR()<<"read slaveLog message count is error!";
        return;
    }
    QLOG_INFO()<<"thirdStart";
    //第三步：发送读记录命令，D2表示上一条接收到的记录条数
    //46 01 （上一条收到的记录条数） CRC
    //设备返回记录数据，最多11条记录，230个字节
    while(this->equParaData.iGetMsgCountCurrentValue()<this->equParaData.GetMsgCount())
    {
        this->equParaData.setFlag();
        slotReadMsg(this->equParaData.iPreMsgCount,1);
        timeout=0;
        while(this->equParaData.getFlag())
        {
            delaySleep1(delayTime);
            timeout++;
            if(timeout>=TIMEOUT)
            {
                timeout=0;
                //启动定时读取时间
                //readTimeTimer->start(readDelayTime);
                //启动断线检测
                //detectDisconnectTimer->start(detectDisconnectTime);
                QLOG_ERROR()<<"In the third read slaveLog message timeout!";
                return;
            }
            //qApp->processEvents();
        }
        emit sigReadSoundSize(equParaData.iGetMsgCountCurrentValue());
        if(this->equParaData.iPreMsgCount <= 0)
        {
            QLOG_INFO()<<"InThirdStageReceiveMsgisZeroandReturn";
            break;
        }
    }
    //循环第三步，直到读完所有记录

    //第四步：发送读记录结束命令
    //46 03 （上一条收到的记录条数） CRC
    //不返回
    slotReadMsg(this->equParaData.iPreMsgCount,3);
    QLOG_INFO()<<"read slaveLog done!";
    //启动定时读取时间
    //readTimeTimer->start(readDelayTime);
    //启动断线检测
    //detectDisconnectTimer->start(detectDisconnectTime);
    emit sigReadSoundSize(msgcount);
    emit closeWindows();
    if(selectStatus)
    {
        //更新日志
        FangHuCangLogItem * tempLog = new FangHuCangLogItem();
        tempLog->name = equParaData.name;
        tempLog->logItem = "读取记录完成，共读取"+QString::number(msgcount)+"条记录";
        equParaData.tempFangHuCangLogList.append(tempLog);
        emit sigInsertFangHuCangLog(equParaData.tempFangHuCangLogList);
        emit sigSlaveReadedMsg(this->equParaData.slaveMsg);
        //清除列表
        equParaData.tempFangHuCangLogList.clear();
        equParaData.slaveMsg.clear();
    }
}
/*
 * 设置语音开始
*/
void Equipment::setVoiceStart(TcpFuncCode func)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=5;
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //需要发送的数据
    uchar data[dataNum]={0};
    //通过功能码获取需要发送的数据包
    createTcpSetVoiceRequest(func,data,dataNum,&datagram);
    //QLOG_INFO()<<"send send send data"<<datagram.toHex();
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);

}
quint64 Equipment::setVoiceData(TcpFuncCode func, QString fileName)
{
    QByteArray datagram=NULL;
    QByteArray fileData=NULL;
    QFile *localFile;    //要发送的文件
    qint64 totalBytes=0;    //数据总大小
    qint64 bytesToWrite=0;   //剩余数据大小
    quint16 loadSize=256;       //每次发送数据的大小
    quint32 addr=0;//已经发送的字节数
    quint16 sendCount=0;
    //需要发送的数据的个数
    quint16 dataNum=268;
    //需要发送的数据
    uchar data[268]={0};
    QString filePath = sFilePath+fileName;
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return 0;
    localFile = new QFile(filePath);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return 0;
    }
    totalBytes = localFile->size();
    QLOG_INFO()<<"totalCount:"<<totalBytes;
    bytesToWrite = totalBytes;//剩余字节数等于总字节数
    fileData = localFile->readAll();
    //QLOG_INFO()<<"音乐文件内容:"<<fileData.toHex();
    addr=0;
    //文件总大小
    emit sigSetWindowsTitle("下载声音");
    emit sigSoundSize(totalBytes);
    emit sigReadSoundSize(addr);
    //QDataStream sendOut(&datagram,QIODevice::WriteOnly);
    while(bytesToWrite>0)
    {
        if(bytesToWrite>loadSize)
        {
            data[0]=1;//有效数据
            data[1]=loadSize&0x00ff;
            data[2]=(loadSize>>8)&0xff;
            sendCount=loadSize;

            data[3]=addr&0xff;
            data[4]=(addr>>8)&0xff;
            data[5]=(addr>>(2*8))&0xff;
            data[6]=(addr>>(3*8))&0xff;
            dataNum = 7+loadSize;
            bytesToWrite -= loadSize;
            for(int i=0;i<loadSize;i++)
            {
                data[i+7]=(uchar)fileData[i];
            }
            fileData.remove(0,loadSize);
            //sendOut<<localFile->read(loadSize);
        }
        else
        {
            data[0]=1;//有效数据
            data[1]=bytesToWrite&0x00ff;
            data[2]=bytesToWrite&0xff00;
            sendCount=bytesToWrite;
            data[3]=addr&0xff;
            data[4]=(addr>>8)&0xff;
            data[5]=(addr>>(2*8))&0xff;
            data[6]=(addr>>(3*8))&0xff;
            dataNum = 7+bytesToWrite;

            for(int i=0;i<bytesToWrite;i++)
            {
                data[i+7]=fileData[i];
            }
            fileData.remove(0,bytesToWrite);
            bytesToWrite -= bytesToWrite;

            //sendOut<<localFile->read(bytesToWrite);
        }
        QLOG_INFO()<<"addr:"<<addr;
        QLOG_INFO()<<"bytesToWrite"<<bytesToWrite;
        //QLOG_INFO()<<"本次报文内容:"<<data;
        //通过功能码获取需要发送的数据包
        quint16 dataLength= sendCount+7;
        createTcpSetVoiceRequest(func,data,dataLength,&datagram);
        //QLOG_INFO()<<"本次报文内容:"<<datagram.toHex();
        //将数据写入缓冲区并发送
        writeDataToBuffer(&datagram);
        addr+=sendCount;
        emit sigReadSoundSize(addr);
        delaySleep1(100);
    }
    delete localFile;
    return totalBytes;
}

void Equipment::setVoiceEnd(TcpFuncCode func,quint32 dataLength)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    static const quint8 dataNum=5;
    //需要发送的数据
    static uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    data[0]=2;//结束
    data[1]=dataLength&0xff;
    data[2]=(dataLength>>8)&0xff;
    data[3]=(dataLength>>16)&0xff;
    data[4]=(dataLength>>24)&0xff;

    quint32 sendTotalByte=data[4];
    sendTotalByte=sendTotalByte<<8|data[3];
    sendTotalByte=sendTotalByte<<8|data[2];
    sendTotalByte=sendTotalByte<<8|data[1];

    QLOG_INFO()<<"sendTotalByte"<<sendTotalByte;
    //通过功能码获取需要发送的数据包
    createTcpSetVoiceRequest(func,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
/*
 * 设置语音
 * 注：总文件大小 <= 196608字节（即768个数据包）
*/
void Equipment::setSound(TcpFuncCode func,QString filename)
{
    quint64 dataLength=0;
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    //停止断线检测
    //detectDisconnectTimer->stop();
    //停止定时读取时间
    //readTimeTimer->stop();

    setVoiceStart(func);//设置声音开始
    dataLength = setVoiceData(func,filename);
    setVoiceEnd(func,dataLength);
    //如果是在应用模板模式 不应该关闭窗口
    if(!usemoduleModel)
    {
        emit closeWindows();
        QLOG_INFO()<<"close window";
    }
    //启动定时读取时间
    //readTimeTimer->start(readDelayTime);
    //启动断线检测
    //detectDisconnectTimer->start(detectDisconnectTime);
}
/*
 * 读取语音开始
 * status 0-首次发送 1－继续发送
*/
void Equipment::readSound(TcpFuncCode func,quint8 status)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    data[0]=status;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
/*
 * 读取语音
*/
void Equipment::ReadSound(TcpFuncCode func)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    this->equParaData.soundSize=0;
    this->equParaData.readSoundSize=0;
    //quint32 readSoundSize;
    static quint8 count=0;
    static quint8 count1=0;
    this->equParaData.setSoundFlag();
    this->equParaData.readSoundDone=0;
    this->equParaData.mp3File.clear();
    count = 0;
    count1=0;
    //停止断线检测
    //detectDisconnectTimer->stop();
    //停止定时读取时间
    //readTimeTimer->stop();

    readSound(func,0);
    while(this->equParaData.getSoundFlag())
    {
        delaySleep1(50);
        count++;
        if(count>20)
        {
            count1=1;
            break;//超时退出
        }
        //跳出此次循环
        if(count1)
        {
            QLOG_INFO()<<"读取语音超时退出!";
            count1=0;
            return;//退出
        }
        if((this->equParaData.soundSize==0))
        {
            QLOG_INFO()<<"读取语音数据大小为0!";
            return;
        }
        //qApp->processEvents();
    }
    emit sigSetWindowsTitle("读取声音");
    emit sigSoundSize(this->equParaData.soundSize);
    emit sigReadSoundSize(this->equParaData.readSoundSize);
    while(!this->equParaData.readSoundDone)
    {
        this->equParaData.soundSize=0;
        this->equParaData.readSoundSize=0;
        this->equParaData.setSoundFlag();
        readSound(func,1);
        count=0;
        while(this->equParaData.getSoundFlag())
        {
            delaySleep1(50);
            count++;
            if(count>200)
            {
                count1=1;
                break;//超时退出
            }
            //跳出此次循环
            if(count1)
            {
                count1=0;
                return;//退出
            }
        }
        emit sigReadSoundSize(this->equParaData.readSoundSize);
        if((this->equParaData.soundSize==0) || (this->equParaData.readSoundSize==0))
        {
            return;
        }
    }
    emit sigReadSoundSize(this->equParaData.readSoundSize);
    emit sigSaveSound(this->GetEquAll());
    //启动定时读取时间
    //readTimeTimer->start(readDelayTime);
    //启动断线检测
    //detectDisconnectTimer->start(detectDisconnectTime);
}
void Equipment::slotReadLed(quint8 dula)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    data[0] = dula;

    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_led,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
void Equipment::slotSetLed(quint8 dula,QString text)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    quint16 dataNum=0;
    //需要发送的数据

    QByteArray textCode = myHelper::codecToGBK(text);
    dataNum = textCode.length()+2;
    const quint16 length = dataNum;
    uchar data[length];
    data[0] = dula;
    data[1] = textCode.length();
    qMemCopy(&data[2],textCode,textCode.length());

    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_led,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
/*
 * 设备声音的md5校验
*/
void Equipment::SetSoundMd5(TcpFuncCode func,QByteArray md5)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=18;
    //需要发送的数据
    uchar data[dataNum]={0};
    qMemCopy(data,md5.data(),16);

    //通过功能码获取需要发送的数据包
    createTcpRequest(func,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
/*
 * 读取声音的md5校验
*/
void Equipment::ReadSoundMd5(TcpFuncCode func)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};

    //通过功能码获取需要发送的数据包
    createTcpRequest(func,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
/*
 * 读取声音音量
*/
void Equipment::ReadSoundVolume(TcpFuncCode func)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    //通过功能码获取需要发送的数据包
    createTcpRequest(func,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
/*
 * 试听声音
*/
void Equipment::PlaySound(TcpFuncCode func,quint8 id)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    data[0]=(uchar)id;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
/*
 * 设置声音音量
*/
void Equipment::SetSoundVolume(TcpFuncCode func,quint8 volume)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    if(volume>16) volume=16;
    data[0]=volume;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
void Equipment::slotReadHyVolume()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSoundVolume(func_tcp_read_hy_volume);
}
/*
 * 设置所有声音的音量
*/
void Equipment::slotSetHyVolume(quint8 volume)
{
    this->equParaData.soundVolume = volume;
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_hy_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_syz_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_whz_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_timeoutWarn_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_timeoutTiShi_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_outDoor_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_openDoor_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_failLock_volume,volume);
    delaySleep1(20);
    SetSoundVolume(func_tcp_set_laDoor_volume,volume);
    delaySleep1(20);
    /*读取声音*/
    slotReadHyVolume();
}
/*
 * 读取所有声音的md5
*/
void Equipment::slotReadAllSoundMd5()
{
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_hy_md5);//
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_syz_md5);//
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_whz_md5);//
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_timeoutWarn_md5);//
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_timeoutTiShi_md5);//
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_outDoor_md5);//
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_openDoor_md5);
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_failLock_md5);
    delaySleep1(20);
    ReadSoundMd5(func_tcp_read_laDoor_md5);
    delaySleep1(20);
}
void Equipment::slotReadHySound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_hy_sound);
}
void Equipment::slotReadSyzSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_syz_sound);
}
void Equipment::slotReadWhzSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_whz_sound);
}
void Equipment::slotReadTimeoutWarnSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_timeoutWarn_sound);
}
void Equipment::slotReadTimeoutTiShiSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_timeoutTiShi_sound);
}
void Equipment::slotReadOutDoorSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_outDoor_sound);
}
void Equipment::slotReadOpenDoorSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_openDoor_sound);
}
void Equipment::slotReadFailLockSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_failLock_sound);
}
void Equipment::slotReadLaDoorSound()
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    ReadSound(func_tcp_read_laDoor_sound);
}
void Equipment::slotSetSound(quint8 id,QString fileName)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    TcpFuncCode func=func_tcp_null;
    switch(id)
    {
    case 0:
        func=func_tcp_set_hy_sound;
        break;
    case 1:
        func=func_tcp_set_syz_sound;
        break;
    case 2:
        func=func_tcp_set_whz_sound;
        break;
    case 3:
        func=func_tcp_set_timeoutWarn_sound;
        break;
    case 4:
        func=func_tcp_set_timeoutTiShi_sound;
        break;
    case 5:
        func=func_tcp_set_outDoor_sound;
        break;
    case 6:
        func=func_tcp_set_openDoor_sound;
        break;
    case 7:
        func=func_tcp_set_failLock_sound;
        break;
    case 8:
        func=func_tcp_set_laDoor_sound;
        break;
    }
    //写入声音文件
    setSound(func,fileName);
    delaySleep1(500);
    //读取md5
    switch(id)
    {
    case 0:
        ReadSoundMd5(func_tcp_read_hy_md5);//
        break;
    case 1:
        ReadSoundMd5(func_tcp_read_syz_md5);//
        break;
    case 2:
        ReadSoundMd5(func_tcp_read_whz_md5);//
        break;
    case 3:
        ReadSoundMd5(func_tcp_read_timeoutWarn_md5);//
        break;
    case 4:
        ReadSoundMd5(func_tcp_read_timeoutTiShi_md5);//
        break;
    case 5:
        ReadSoundMd5(func_tcp_read_outDoor_md5);//
        break;
    case 6:
        ReadSoundMd5(func_tcp_read_openDoor_md5);
        break;
    case 7:
        ReadSoundMd5(func_tcp_read_failLock_md5);
        break;
    case 8:
        ReadSoundMd5(func_tcp_read_laDoor_md5);
        break;
    default:
        break;
    }
}
void Equipment::slotSetMd5(quint8 id,QString md5)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    TcpFuncCode func=func_tcp_null;
    switch(id)
    {
    case 0:
        func=func_tcp_set_hy_md5;
        break;
    case 1:
        func=func_tcp_set_syz_md5;
        break;
    case 2:
        func=func_tcp_set_whz_md5;
        break;
    case 3:
        func=func_tcp_set_timeoutWarn_md5;
        break;
    case 4:
        func=func_tcp_set_timeoutTiShi_md5;
        break;
    case 5:
        func=func_tcp_set_outDoor_md5;
        break;
    case 6:
        func=func_tcp_set_openDoor_md5;
        break;
    case 7:
        func=func_tcp_set_failLock_md5;
        break;
    case 8:
        func=func_tcp_set_laDoor_md5;
        break;
    default:
        break;
    }
    QByteArray Md5 = myHelper::hexStringtoByteArray(md5);
    //写入声音md5
    SetSoundMd5(func,Md5);
}
void Equipment::slotChangeSlaveSound(quint8 id,QString fileName,QString md5)
{

    //停止断线检测
    //detectDisconnectTimer->stop();
    //停止定时读取时间
    //readTimeTimer->stop();

    slotSetMd5(id,md5);
    slotSetSound(id,fileName);
    //启动定时读取时间
    //readTimeTimer->start(readDelayTime);
    //启动断线检测
    //detectDisconnectTimer->start(detectDisconnectTime);
}
void Equipment::slotPlaySound(quint8 id)
{
    if(this->equParaData.slaveStatus.connectStatus!=1)
        return;
    PlaySound(func_tcp_play_sound,id);
}
void Equipment::slotUseModule(paraData *temp)
{
    quint16 size=1;
    usemoduleModel=1;
    emit sigSetWindowsTitle("应用模板");
    emit sigUseModule(16);
    //更新照明参数
    useModule = 1;
    if(temp->zhaoMing.Model != equParaData.zhaoMing.Model ||
            temp->zhaoMing.startHour != equParaData.zhaoMing.startHour||
            temp->zhaoMing.startMinute != equParaData.zhaoMing.startMinute||
            temp->zhaoMing.endHour != equParaData.zhaoMing.endHour||
            temp->zhaoMing.endMinute!= equParaData.zhaoMing.endMinute)

    {
        slotSetLineAndAdLinePara(&temp->zhaoMing);
        while(useModule)
        {
            //qApp->processEvents();
            delaySleep1(50);
        }
    }
    emit sigUseModuleSize(1);
    //更新防护舱参数
    useModule = 1;
    slotSetWorkPara(&temp->fangHuCang);
    while(useModule)
    {
        delaySleep1(50);
        //qApp->processEvents();
    }
    emit sigUseModuleSize(2);
    //更新报警参数

    if(equParaData.getTwoMainVer()>27||equParaData.slaveType==1)
    {
        useModule = 1;
        slotSetAlarmPara(&temp->alarmPara);
        while(useModule)
        {
            delaySleep1(50);
            //qApp->processEvents();
        }
    }
    emit sigUseModuleSize(3);
    useModule = 1;
    //更新LED内容
    for(int i=0;i<3;i++)
    {

        slotSetLed(i,temp->ledText[i]);
        while(useModule)
        {
            delaySleep1(50);
        }
        useModule = 1;
        slotReadLed(i);
        while(useModule)
        {
            delaySleep1(50);
        }
        useModule = 1;
        emit sigUseModuleSize(3+size++);
    }
    //设置声音音量
    useModule = 1;
    slotSetHyVolume(temp->soundVolume);
    while(useModule)
    {
        delaySleep1(50);
    }
    emit sigUseModuleSize(7);
    useModule = 1;
    size=1;
    //更新语音内容
    for(int i=0;i<9;i++)
    {
        if(temp->soundMd5[i] == "")
        {
            emit sigUseModuleSize(7+size++);
            continue;
        }
        if(temp->soundMd5[i].toHex() == this->equParaData.soundMd5[i].toHex())
        {
            emit sigUseModuleSize(7+size++);
            continue;
        }
        useModule = 1;
        //发送更换声音信号
        emit sigChangeSoundFromModule(temp,i);
        //等待设置完成
        while(useModule)
        {
            delaySleep2(50);
        }
        //useModule = 1;
        emit sigUseModuleSize(7+size++);
    }
    usemoduleModel=0;
    emit closeWindows();
    //QLOG_INFO()<<"close window";
}
void Equipment::slotSetAlarmPara(paraAlarm* alarmPara)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=14;
    //需要发送的数据
    uchar data[dataNum]={0};
    switch (equParaData.slaveType)
    {
    case 0://防护舱
        data[0] = (quint8)alarmPara->btnAlarmEnable;
        data[1] = (quint8)alarmPara->cutAlarmEnable;
        data[2] = (quint8)alarmPara->zhengDongAlarmEnable;
        data[3] = (quint8)alarmPara->yanWuAlarmEnable;
        data[4] = (quint8)alarmPara->boLiAlarmEnable;
        data[5] = (quint8)alarmPara->shuiQinAlarmEnable;
        data[6] = (quint8)alarmPara->tempAlarmEnable;
        data[7] = 0;
        data[8] = 0;
        break;
    case 1://加钞间
        data[0] = (quint8)alarmPara->btnAlarmEnable;
        data[1] = (quint8)alarmPara->cutAlarmEnable;
        data[2] = (quint8)alarmPara->zhengDongAlarmEnable;
        data[3] = (quint8)alarmPara->yanWuAlarmEnable;
        data[4] = (quint8)alarmPara->boLiAlarmEnable;
        data[5] = (quint8)alarmPara->shuiQinAlarmEnable;
        data[6] = (quint8)alarmPara->tempAlarmEnable;
        data[7] = (quint8)alarmPara->doorCiAlarmEnable;
        data[8] = (quint8)alarmPara->existManAlarmEnable;
        break;
    case 0xff:
        break;
    default:
        break;
    }

    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_alarmPara,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);

    //设置完后再读取一次
    slotGetAlarmPara();
}
//读取报警参数
void Equipment::slotGetAlarmPara()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_alarmPara,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//读取报警参数
void Equipment::slotGetAlarmStatus()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_alarmStatus,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
void Equipment::slotClearAlarm()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_clear_alarm,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
void Equipment::setZhaoMingPara(paraZhaoMing *par)
{
    slotSetLineAndAdLinePara(par);
}
void Equipment::setWorkPara(paraFangHuCang *par)
{
    slotSetWorkPara(par);
}
void Equipment::slotAutoAddTimeSec()
{
    equParaData.slaveTime = equParaData.slaveTime.addSecs(1);
    emit sigRefreshTime(this->GetEquAll());
}
/****************************加钞间函数********************************/
//设置系统报警状态 设防和撤防按钮
void Equipment::slotSetSlaveAlarmStatus(quint8 status)
{
    equParaData.fangHuCang.systemAlarmStatus = status;
    slotSetWorkPara(&equParaData.fangHuCang);
}
//返回正常状态状态
void Equipment::slotReturnNormalStatus()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_status,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//进入增加钥匙状态
void Equipment::slotSetAddKeyStatus()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    data[0]=1;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_status,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//进入增加主钥匙状态
void Equipment::slotSetAddMainKeyStatus()
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    data[0]=2;
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_set_status,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//删除钥匙
void Equipment::slotDeleteKey(int id)
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=8;
    uchar *data = (uchar*)equParaData.keyList.at(id).data();
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_delete_key,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//读钥匙
void Equipment::slotReadKey()
{
    equParaData.readedKey=1;
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=2;
    //需要发送的数据
    uchar data[dataNum]={0};
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_read_key,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
//写入主钥匙
void Equipment::slotWriteMainKey(uchar keyData[16])
{
    static QByteArray datagram=NULL;
    //需要发送的数据的个数
    const quint16 dataNum=17;
    //需要发送的数据
    uchar data[dataNum]={0};
    data[0] = 0xC1;
    for(int i=0;i<16;i++)
    {
        data[1+i] = keyData[i];
    }
    //通过功能码获取需要发送的数据包
    createTcpRequest(func_tcp_write_mainKey,data,dataNum,&datagram);
    //将数据写入缓冲区并发送
    writeDataToBuffer(&datagram);
}
