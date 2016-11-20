#include "datastruct.h"
#include <QStringList>
#include <QString>
#include <private/qutfcodec_p.h>
#include <QObject>
#include <QDateTime>
#include "../myhelper.h"

static void slaveInsetOneLog(paraData *pdat,QString log)
{
    //更新日志
    FangHuCangLogItem * tempLog = new FangHuCangLogItem();
    tempLog->name = pdat->name+"("+pdat->net.macAddr+")";
    tempLog->logItem = log;
    pdat->tempFangHuCangLogList.append(tempLog);
}
/*
 * 获取版本号
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static QString GetSlaveVersion(char *buffer,quint8 id)
{
    QString retValue="";
    QString str;
    for(int i=0;i<2;i++)
    {
        str = QString::number((quint8)buffer[id+i]);
        retValue.append(str);
        retValue.append(".");
    }
    str = QString::number((quint8)buffer[2+id]);
    retValue.append(str);
    return retValue;

}
//获取IP地址
/*
 * 获取IP地址
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static QString GetIp(char *buffer,quint8 id)
{
    QString retValue="";
    QString str;
    for(int i=0;i<3;i++)
    {
        str = QString::number((quint8)buffer[id+i]);
        retValue.append(str);
        retValue.append(".");
    }
    str = QString::number((quint8)buffer[id+3]);
    retValue.append(str);
    return retValue;
}
/*
 * 获取指定字节并转化为quint8
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetOneByteToQuint8(char *buffer,quint8 id)
{
    quint8 retValue=0;
    retValue=(quint8)buffer[id];
    return retValue;
}
/*
 * 获取网关地址
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static QString GetGateWay(char *buffer,quint8 id)
{
    QString retValue="";
    retValue = GetIp(buffer,id);
    return retValue;
}
//获取子网俺码
/*
 * 获取子网俺码
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static QString GetSubnetMask(char *buffer,quint8 id)
{
    QString retValue="";
    retValue = GetIp(buffer,id);
    return retValue;
}
//获取修订版本
/*
 * 获取修订版本
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetXiuDingVer(char *buffer,quint8 id)
{
    quint8 retValue=0;
    retValue=GetOneByteToQuint8(buffer,id);
    return retValue;
}
//获取设备类型
/*
 * 获取设备类型
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetPeripheralModel(char *buffer,quint8 id)
{
    quint8 retValue=0;
    retValue=GetOneByteToQuint8(buffer,id);
    return retValue;
}
//获取mac地址
/*
 * 获取mac地址
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static QString GetMacAddr(char *buffer,quint8 id)
{
    QString retValue="";
    QString str;
    for(int i=0;i<6;i++)
    {
        str = QString("%1").arg(buffer[id+i]&0xFF, 2, 16, QLatin1Char('0'));
        retValue.append(str);
        //retValue.append(":");
    }
    //str = QString("%1").arg(buffer[id+5]&0xFF, 2, 16, QLatin1Char('0'));
    //retValue.append(str);
    return retValue;
}
//获取设备名称
/*
 * 获取设备名称
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static QString GetSlaveName(char *buffer,quint8 id)
{
    QString retValue="";
    char temp[30];
    quint16 ichar;
    QChar qchararray[15];
    int j=0;

    for(int i=0;i<30;i++)
    {
       temp[i]=buffer[i+id];

    }

    for(int i=0;i<30;i++)
    {
        ichar=0;
        ichar = temp[i++];
        ichar = (ichar&0xff)|(temp[i]<<8);
        //如果unicode等于零 则变为空格
        if(ichar>0||ichar<0)
        {
            qchararray[j++]=ichar;
        }
        else
        {
            qchararray[j++]=0x20;
        }
    }
    retValue = retValue.setUnicode(qchararray,15);
    retValue = retValue.simplified();//去掉首尾的空格

    return retValue;
}
//获取远程IP地址
/*
 * 获取远程IP地址
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static QString GetRemoteIp(char *buffer,quint8 id)
{
    QString retValue="";
    retValue = GetIp(buffer,id);
    return retValue;
}
//获取联网模式
/*
 * 获取联网模式
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetNetworkModel(char  *buffer,quint8 id)
{
    quint8 retValue=0;
    retValue=GetOneByteToQuint8(buffer,id);
    return retValue;
}
//获取设备类型
/*
 * 获取设备类型
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetSlaveModel(char  *buffer,quint8 id)
{
    quint8 retValue=0;
    retValue=GetOneByteToQuint8(buffer,id);
    return retValue;
}
/*
 * 获取两个字节的内容转化成quint16低字节在低位
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint16 GetTwoBytesToQuint16(char *buffer,quint8 id)
{
    quint16 retValue=0;
    retValue = (quint8)buffer[id+1];
    retValue = (retValue<<8)|((quint8)buffer[id]);
    return retValue;
}
/*
 * 获取端口号
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint16 GetPort(char *buffer,quint8 id)
{
    quint16 retValue=0;
    retValue = GetTwoBytesToQuint16(buffer,id);
    return retValue;
}
/*
 * 获取温度
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint16 GetTemperature(char *buffer,quint8 id)
{
    quint16 retValue=0;
    retValue = GetTwoBytesToQuint16(buffer,id);
    return retValue;
}
/*
 * 获取湿度
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint16 GetHumidity(char *buffer,quint8 id)
{
    quint16 retValue=0;
    retValue = GetTwoBytesToQuint16(buffer,id);
    return retValue;
}
/*
 * 获取设备状态类型
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetSlaveSubModel(char *buffer,quint8 id)
{
    quint8 retValue=0;
    retValue=GetOneByteToQuint8(buffer,id);
    return retValue;
}

/*
 * 获取设备状态值
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetSlaveSubModelStatus(char *buffer,quint8 id)
{
    quint8 retValue=0;
    retValue=GetOneByteToQuint8(buffer,id);
    return retValue;
}
/*
 * 获取设备时间
 *
 * @buffer 需要提取数据的数据包
 * @id 在数据包中的偏移
 *
 * */
static quint8 GetSlaveTime(char *buf,quint8 id)
{
    quint8 retValue=0;
    uchar time=buf[id];
    retValue=(quint8)(time&0x0f)+(quint8)((time&0xf0)>>4)*10;
    return retValue;
}

/*返回0－正常； 1 crc－校验错误
 *
 *
*/
quint8 Func_Udp_01(char* buf, paraData *pdat)
{
    quint8 retValue=0;
    char * temp=buf;
    QString Version;
    quint8  xiuDingVer;
    QString macAddr;
    QString slaveName;
    quint8 slaveModel;
    QString ip;
    QString gateWay;
    QString subnetMask;
    quint8  networkModel;//连网方式 0－服务器
    quint16 port;
    quint8 func=(quint8)buf[0]-100;
    //QString mac2Addr;
    //解密
    getRealValue(buf,1,69);

    //temp=buf.data();
    //uchar * uctemp=(uchar *)temp;
    uchar crc = getCRC((uchar *)buf,71);

    if(crc!=(uchar)buf[71])
    {
        retValue=1;//校验错误 返回
    }
    else
    {
        macAddr=GetMacAddr(temp,5);
        slaveName=GetSlaveName(temp,11);
        Version = GetSlaveVersion(temp,2);
        if(func == 1)
        {
            ip=GetIp(temp,41);
        }
        //peripheralModel = GetPeripheralModel(temp,4);
        gateWay = GetGateWay(temp,49);
        subnetMask = GetSubnetMask(temp,45);
        networkModel = GetNetworkModel(temp,53);
        slaveModel = GetSlaveModel(temp,1);
        port = GetPort(temp,58);
        xiuDingVer = GetXiuDingVer(temp,69);

        Version.append(".");
        Version.append(QString::number(xiuDingVer));

        pdat->net.ip=ip;
        pdat->net.gateWay=gateWay;
        pdat->net.networkModel=networkModel;
        pdat->net.subnetMask=subnetMask;
        pdat->net.tcpPort=port;
        pdat->net.macAddr=macAddr;
        pdat->name=slaveName;
        pdat->version=Version;
        pdat->slaveType=slaveModel;
        //版本获取
        pdat->xiuDingVer = xiuDingVer;
        pdat->mainVer = (quint8)buf[2];
        pdat->secondVer = (quint8)buf[3];
        pdat->waiSheModel = (quint8)buf[4];

        QLOG_INFO()<<"macAddr:"<<macAddr;
        QLOG_INFO()<<"slaveName:"<<slaveName;
        QLOG_INFO()<<"Version:"<<Version;
        QLOG_INFO()<<"ip:"<<ip;
    }
    return retValue;
}
/*
 * 返回值0 处理正常 1 crc校验错误
*/
quint8 Func_tcp_1(char *buf,paraData *pdat)
{
    quint8 retValue=0;

    QString Version;
    quint8  xiuDingVer;
    QString macAddr;
    QString slaveName;
    quint8 slaveModel;
    QString ip;
    QString gateWay;
    QString subnetMask;
    quint8  networkModel;//连网方式 0－服务器
    QString remoteIp;
    quint16 port;
    //QString mac2Addr;

    uchar crc = getCRC((uchar *)buf,63);
    if(crc==(uchar)buf[63])
    {
        slaveModel = GetSlaveModel(buf,1);
        Version = GetSlaveVersion(buf,2);

        macAddr = GetMacAddr(buf,5);
        slaveName = GetSlaveName(buf,11);
        ip = GetIp(buf,41);
        subnetMask = GetSubnetMask(buf,45);
        gateWay = GetGateWay(buf,49);
        networkModel = GetNetworkModel(buf,53);
        remoteIp = GetIp(buf,54);
        port = GetPort(buf,58);
        xiuDingVer = GetXiuDingVer(buf,60);
        Version.append(".");
        Version.append(QString::number(xiuDingVer));

        //版本获取
        pdat->xiuDingVer = xiuDingVer;
        pdat->mainVer = (quint8)buf[2];
        pdat->secondVer = (quint8)buf[3];
        pdat->waiSheModel = (quint8)buf[4];
        //直接添加时不更新ip地址 因为可能是远程ip
        //pdat->net.ip=ip;
        pdat->net.gateWay=gateWay;
        pdat->net.networkModel=networkModel;
        pdat->net.subnetMask=subnetMask;
        pdat->net.tcpPort=port;
        pdat->net.macAddr=macAddr;
        pdat->net.remoteIp = remoteIp;
        pdat->name=slaveName;
        pdat->version=Version;
        pdat->slaveType=slaveModel;

        QLOG_INFO()<<"macAddr:"<<macAddr
                    <<"slaveName:"<<slaveName
                    <<"Version:"<<Version
                    <<"ip:"<<ip;
    }
    else//校验错误 返回
    {
        retValue=1;
    }

    return retValue;
}
/*
 * 返回值0 处理正常 1 crc校验错误
*/
quint8 FunctTcp1(char *buf,paraData *pdat)
{
    quint8 retValue=0;

    QString Version;
    quint8  xiuDingVer;
    QString macAddr;
    QString slaveName;
    quint8 slaveModel;
    QString ip;
    QString gateWay;
    QString subnetMask;
    quint8  networkModel;//连网方式 0－服务器
    QString remoteIp;
    quint16 port;
    //QString mac2Addr;

    uchar crc = getCRC((uchar *)buf,63);
    if(crc==(uchar)buf[63])
    {
        slaveModel = GetSlaveModel(buf,1);
        Version = GetSlaveVersion(buf,2);

        macAddr = GetMacAddr(buf,5);
        slaveName = GetSlaveName(buf,11);
        ip = GetIp(buf,41);
        subnetMask = GetSubnetMask(buf,45);
        gateWay = GetGateWay(buf,49);
        networkModel = GetNetworkModel(buf,53);
        remoteIp = GetIp(buf,54);
        port = GetPort(buf,58);
        xiuDingVer = GetXiuDingVer(buf,60);
        Version.append(".");
        Version.append(QString::number(xiuDingVer));

        //版本获取
        pdat->xiuDingVer = xiuDingVer;
        pdat->mainVer = (quint8)buf[2];
        pdat->secondVer = (quint8)buf[3];
        pdat->waiSheModel = (quint8)buf[4];
        //直接添加时不更新ip地址 因为可能是远程ip
        pdat->net.ip=ip;
        pdat->net.gateWay=gateWay;
        pdat->net.networkModel=networkModel;
        pdat->net.subnetMask=subnetMask;
        pdat->net.tcpPort=port;
        pdat->net.macAddr=macAddr;
        pdat->net.remoteIp = remoteIp;
        pdat->name=slaveName;
        pdat->version=Version;
        pdat->slaveType=slaveModel;

        QLOG_INFO()<<"macAddr:"<<macAddr
                    <<"slaveName:"<<slaveName
                    <<"Version:"<<Version
                    <<"ip:"<<ip;
    }
    else//校验错误 返回
    {
        retValue=1;
    }

    return retValue;
}
quint8 Func_tcp_2(char *buf,paraData *pdat)
{
    quint8 retValue=0;
    return retValue;
}
/*
 *读取设备时间返回报文处理函数
 */
quint8 Func_tcp_3(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    QDate *date=new QDate();
    QTime *time=new QTime();

    int year=0;
    int month=0;
    int day=0;
    int hour=0;
    int minutes=0;
    int second=0;
    int week=1;
    uchar crc = getCRC((uchar *)buf,15);
    if(crc==(uchar)buf[15])
    {
        year=GetSlaveTime(buf,1);
        month=GetSlaveTime(buf,2);
        day=GetSlaveTime(buf,3);
        hour=GetSlaveTime(buf,4);
        minutes=GetSlaveTime(buf,5);
        second=GetSlaveTime(buf,6);
        week=GetSlaveTime(buf,7);
        date->setDate(year,month,day);
        time->setHMS(hour,minutes,second);
        pdat->slaveDate = *date;
        pdat->slaveTime = *time;
        //QLOG_INFO()<<pdat->slaveDate.toString("yy年MM月d日");
        //QLOG_INFO()<<pdat->slaveTime.toString("hh时mm分ss秒");
    }
    else
    {
        retValue=1;
    }
    delete time;
    delete date;
    return retValue;
}
/*
 * 设置时间返回值
*/
quint8 Func_tcp_4(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置时间结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 *解析设备工作状态数据
 *
 *  D1:1-门开；0-门关
    D2:照明灯。1-打开
    D3:广告灯。1-打开
    D4:风扇。1-启动
    D5:1-使用；0-空闲
    D6:1-启用；0-停用
    D7:按钮报警。1-报警
    D8:1-锁闭；0-解锁
    D9:1－防切割报警 0－未报
    D10:温度低8位
    D11:温度高8位
    D12:湿度低8位
    D13:湿度高8位
 * */
quint8 Func_tcp_5(char *buf, paraData *pdat)
{
    quint8 retValue=0;

    quint8 gateStatus=0;
    quint8 zhaomingStatus=0;
    quint8 guangGaoStatus=0;
    quint8 fanStatus=0;
    quint8 usedStatus=0;
    quint8 lockStatus=0;
    quint8 qiegeWarning=0;
    quint8 enableOrNotStatus=0;
    quint8 btnAlarmStatus=0;
    quint16 temperature=0;
    quint16 humdiy=0;
    //加钞间参数
    quint8 workStatus=0;
    quint8 alarmStatus=0;
    quint8 crc = getCRC((uchar *)buf,15);
    if(crc==(uchar)buf[15])
    {
        gateStatus=(quint8)buf[1];
        zhaomingStatus=(quint8)buf[2];
        switch (pdat->slaveType)
        {
        case 0:
            guangGaoStatus=(quint8)buf[3];
            fanStatus=(quint8)buf[4];
            usedStatus=(quint8)buf[5];
            enableOrNotStatus=(quint8)buf[6];
            //参数写入
            pdat->slaveStatus.dengxiangStatus=guangGaoStatus;
            pdat->slaveStatus.fanStatus=fanStatus;
            pdat->slaveStatus.usedStatus=usedStatus;
            pdat->slaveStatus.fanghucangStatus=enableOrNotStatus;
            break;
        case 1:
            workStatus=(quint8)buf[5];
            alarmStatus=(quint8)buf[6];
            //参数写入
            pdat->slaveStatus.workStatus = workStatus;
            pdat->slaveStatus.alarmStatus = alarmStatus;
            break;
        case 0xff:
            break;
        }
        btnAlarmStatus=(quint8)buf[7];//按钮报警
        lockStatus = (quint8)buf[8];
        qiegeWarning = (quint8)buf[9];
        temperature=GetTemperature(buf,10);
        humdiy=GetHumidity(buf,12);

        pdat->slaveStatus.gateStatus=gateStatus;
        pdat->slaveStatus.zhaomingStatus=zhaomingStatus;
        pdat->slaveStatus.buttonAlarmStatus=btnAlarmStatus;
        pdat->slaveStatus.temperature=temperature;
        pdat->slaveStatus.humidity = humdiy;
        pdat->slaveStatus.lockStatus = lockStatus;
        pdat->slaveStatus.cutAlarmStatus = qiegeWarning;
/*
        QLOG_INFO()<<"门状态:"<<gateStatus
                    <<"照明状态:"<<zhaomingStatus
                    <<"广告状态:"<<guangGaoStatus
                    <<"风扇状态:"<<fanStatus
                    <<"使用状态:"<<usedStatus
                    <<"闭锁状态:"<<lockStatus
                    <<"切割报警:"<<qiegeWarning
                    <<"防护舱状态:"<<enableOrNotStatus
                    <<"温度:"<<temperature
                    <<"湿度:"<<humdiy;*/

    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 设置照明灯和广告灯参数返回值
*/
quint8 Func_tcp_6(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置照明灯和广告灯参数结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 获取照明灯和广告灯参数
 *  D1: 开启方式 开启方式：0 – 停用；1 - 光控；2 – 时控；3 – 光控+时控
 *  D2: 打开-小时
 *  D3: 打开-分钟
 *  D4: 关闭-小时
 *  D5: 关闭-分钟
 *  D6: 1 - 有人灯亮
*
*/
quint8 Func_tcp_7(char *buf, paraData *pdat)
{
    quint8 retValue=0;

    quint8 openModel=0;
    quint8 openH=0;
    quint8 openM=0;
    quint8 closeH=0;
    quint8 closeM=0;
    quint8 man=0;

    quint8 crc = getCRC((uchar *)buf,11);
    if(crc==(uchar)buf[11])
    {
        openModel=(quint8)buf[1];
        openH=GetSlaveTime(buf,2);;
        openM=GetSlaveTime(buf,3);;
        closeH=GetSlaveTime(buf,4);;
        closeM=GetSlaveTime(buf,5);;
        man = (quint8)buf[6];
        pdat->zhaoMing.Model = openModel;
        pdat->zhaoMing.startHour = openH;
        pdat->zhaoMing.startMinute = openM;
        pdat->zhaoMing.endHour = closeH;
        pdat->zhaoMing.endMinute = closeM;
        pdat->zhaoMing.manEnable = man;

        QLOG_INFO()<<"openModel:"<<openModel
                    <<"startHour:"<<openH
                    <<"openM:"<<openM
                    <<"closeH:"<<closeH
                    <<"closeM:"<<closeM
                    <<"man:"<<man;

    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 设置工作参数返回值
*/
quint8 Func_tcp_8(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置工作参数结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
        if(pdat->slaveStatus.comError)
        {
            QLOG_ERROR()<<"set workPara error! ErrorCode="<<pdat->slaveStatus.comError;
        }
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 获取工作参数（16字节）
 *  D1: 开锁延时
    D2: 业务时间
    D3: 超时提醒时间
    D4: 提醒间隔（固定0）
    D5: 无人开锁时间
    D6: 备用0
    D7: 风扇启动温度
    D8: 人体信号输入方式
    D9: 人体设备类型
    D10: 锁具
    D11: 正常是否上锁

*/
quint8 Func_tcp_9(char *buf, paraData *pdat)
{
    quint8 retValue=0;

    quint8 openLockDelayTime=0;
    quint8 yewuTime=0;
    quint8 outtimeWarm=0;
    quint8 warmDelayTime=0;
    quint8 nomanOpenLockTime=0;
    quint8 fanStartTemperature=0;
    quint8 manSigInputModel=0;
    quint8 manSlaveModel=0;
    quint8 locker=0;
    quint8 normalLockorNot=0;
    quint8 fangqie=0;

    //  加钞间参数
    quint8 UserNum;             //D2 刷卡人数（1-10人）N+1，如果设置为0表示单卡进出，不为0表示多卡
    quint8 systemAlarmStatus;   //D3 设置系统报警状态：0 - 撤防；1 - 设防
    quint8 setGuardDelayTime;   //D4 设防延时时间（1-90秒钟）
    quint8 isMonitorOrNot;      //D5 是否有人值守：0 - 无人值守；1 - 有人值守
    quint8 inDoorModel;         //D6: 进门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码
    quint8 outDoorModel;        //D7: 出门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码；5-按钮

    quint8 crc = getCRC((uchar *)buf,15);
    if(crc==(uchar)buf[15])
    {

        switch (pdat->slaveType)
        {
        case 0:
            yewuTime=(quint8)buf[2];
            outtimeWarm=(quint8)buf[3];
            warmDelayTime=(quint8)buf[4];
            nomanOpenLockTime=(quint8)buf[5];
            fanStartTemperature=(quint8)buf[7];
            manSigInputModel=(quint8)buf[8];
            manSlaveModel=(quint8)buf[9];

            normalLockorNot=(quint8)buf[11];
            fangqie = (quint8)buf[12];

            pdat->fangHuCang.OptBussinessTime = yewuTime;
            pdat->fangHuCang.timeOutRemind = outtimeWarm;
            pdat->fangHuCang.warnningDelayTime = warmDelayTime;
            pdat->fangHuCang.noManOpenLockTime = nomanOpenLockTime;
            pdat->fangHuCang.fanRunTemperature = fanStartTemperature;
            pdat->fangHuCang.signalModel = manSigInputModel;
            pdat->fangHuCang.peopleEquModel = manSlaveModel;

            pdat->fangHuCang.kongCangLockorNot = normalLockorNot;
            pdat->fangHuCang.fangQiewarnning = fangqie;
            break;
        case 1:
            UserNum=(quint8)buf[2];             //D2 刷卡人数（1-10人）N+1，如果设置为0表示单卡进出，不为0表示多卡
            systemAlarmStatus=(quint8)buf[3];   //D3 设置系统报警状态：0 - 撤防；1 - 设防
            setGuardDelayTime=(quint8)buf[4];   //D4 设防延时时间（1-90秒钟）
            isMonitorOrNot=(quint8)buf[5];      //D5 是否有人值守：0 - 无人值守；1 - 有人值守
            inDoorModel=(quint8)buf[6];         //D6: 进门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码
            outDoorModel=(quint8)buf[7];        //D7: 出门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码；5-按钮

            pdat->fangHuCang.userNum = UserNum;
            pdat->fangHuCang.systemAlarmStatus = systemAlarmStatus;
            pdat->fangHuCang.setGuardDelayTime = setGuardDelayTime;
            pdat->fangHuCang.isMonitorOrNot = isMonitorOrNot;
            pdat->fangHuCang.inDoorModel = inDoorModel;
            pdat->fangHuCang.outDoorModel = outDoorModel;
            break;
        case 0xff:
            break;
        }

        /***公共参数***/
        openLockDelayTime=(quint8)buf[1];
        locker=(quint8)buf[10];

        pdat->fangHuCang.openLockTime = openLockDelayTime;
        pdat->fangHuCang.lockModel = locker;
        /*
        QLOG_INFO()<<"开锁延时:"<<openLockDelayTime
                  <<"业务时间:"<<yewuTime
                  <<"超时提醒时间:"<<outtimeWarm
                  <<"提醒间隔（固定0）:"<<warmDelayTime
                  <<"无人开锁时间:"<<nomanOpenLockTime
                  <<"风扇启动温度:"<<fanStartTemperature
                  <<"人体信号输入方式:"<<manSigInputModel
                  <<"人体设备类型:"<<manSlaveModel
                  <<"锁具:"<<locker
                  <<"正常是否上锁:"<<normalLockorNot;*/

    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 启用/禁用控制器（4字节）
*/
quint8 Func_tcp_10(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置工作参数结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
quint8 Func_tcp_15(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    return retValue;
}
quint8 Func_tcp_16(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    return retValue;
}
quint8 Func_tcp_65(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置工作参数结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
        retValue = (quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 远程开门（4字节）返回值
*/
quint8 Func_tcp_66(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置工作参数结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
        retValue = (quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 远程锁闭（4字节）返回值
*/
quint8 Func_tcp_68(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置工作参数结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 读取控制器内记录总条数（4字节）返回值 0x45
*/
quint8 Func_tcp_69(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    quint32 msgNum=0;
    quint8 crc = getCRC((uchar *)buf,7);
    if(crc==(uchar)buf[7])
    {
        for(int i=0;i<6;i++)
        {
            msgNum = msgNum<<8;
            msgNum = msgNum|(quint8)buf[6-i];
        }
        pdat->setMsgCount(msgNum);

        QLOG_INFO()<<"Message Number:"<<pdat->GetMsgCount();
    }
    else
    {
        retValue=1;
        pdat->setMsgCount(0);
    }

    return retValue;
}
/*
 * 读取控制器内记录（最多230字节）返回值 0x46
*/
quint8 Func_tcp_70(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    quint32 addedMsgCount=0;//添加的记录的数量
    quint32 msgCount=0;//设备返回的数量
    quint8 exist=0;
    quint8 length = (quint8)buf[1];
    pdat->emitslaveMsg.clear();

    quint8 crc = getCRC((uchar *)buf,length+1);
    if(crc==(uchar)buf[length+1])
    {
        //提取记录
        //计算记录的条数
        pdat->iPreMsgCount = 0;
        msgCount = (length-8)/20;
        QLOG_INFO()<<"receivemsgCount:"<<msgCount;
        for(int i=0;i<msgCount;i++)
        {
            SlaveMsg *msg = new SlaveMsg();
            char * buffer = &buf[20*i+4];
            //提取卡号
            for(int j=0;j<8;j++)
            {
                msg->cardNum[j] = buffer[j];
            }
            //提取时间
            for(int j=0;j<4;j++)
            {
                msg->time = msg->time<<8;
                msg->time = msg->time |(quint8)buffer[11-j];
            }
            //提取类型
            msg->msgType = (quint8)buffer[12];
            msg->fushu1 = (quint8)buffer[14];
            msg->fushu2 = (quint8)buffer[15];
            /**************先判断记录里面是否已经存在**************/
            for(int k=0;k<pdat->slaveMsg.count();k++)
            {
                if(pdat->slaveMsg.at(k) == msg)
                {
                    exist=1;
                    break;
                }
            }

            if(0 == exist)
            {
                pdat->slaveMsg.append(msg);
                pdat->emitslaveMsg.append(msg);
                addedMsgCount++;
            }
            else
            {
                exist = 0;
                QLOG_INFO()<<"delete repeat slaveLog";
                delete msg;
            }

        }
        pdat->iPreMsgCount = addedMsgCount;
        pdat->iGetMsgCountAdd(addedMsgCount);

    }
    else
    {
        retValue=1;
    }

    return retValue;
}
/*
 * 设置LED显示内容
*/
quint8 Func_tcp_71(char *buf,paraData *pdat)
{
    quint8 retValue=0;
    //防切报警返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        QLOG_INFO()<<"set led text right";
    }
    else
    {
        retValue=1;
    }
    //QLOG_INFO()<<"set led text return";
    return retValue;
}
/*
 * 读取LED显示内容
*/
quint8 Func_tcp_72(char *buf,paraData *pdat)
{
    QByteArray ledArray;
    quint8 retValue=0;
    //QLOG_INFO()<<"read led text return";
    quint8 dataLength = (quint8)buf[3];
    quint8 dula = (quint8)buf[2];
    quint8 crc = getCRC((uchar *)buf,dataLength+4);
    if(crc==(uchar)buf[dataLength+4])
    {
        //QLOG_INFO()<<"read led text return";
        for(int i=0;i<dataLength;i++)
        {
            ledArray[i]=buf[i+4];
            QLOG_INFO()<<"read led text return";
        }
        //QLOG_INFO()<<"read led text return";
        pdat->ledArray[dula]=ledArray;
        pdat->ledText[dula]=myHelper::decodecToGBK(ledArray);
        //QLOG_INFO()<<"read led text return";
    }
    else
    {
        retValue=1;
    }

    return retValue;
}
/*
 * 读取报警参数 0x49
*/
quint8 Func_tcp_73(char *buf,paraData *pdat)
{
    //QByteArray ledArray;
    quint8 retValue=0;
    QLOG_INFO()<<"read alarmPara text return";

    quint8 crc = getCRC((uchar *)buf,15);
    if(crc==(uchar)buf[15])
    {
        pdat->alarmPara.btnAlarmEnable = (quint8)buf[1];
        pdat->alarmPara.cutAlarmEnable = (quint8)buf[2];
        pdat->alarmPara.zhengDongAlarmEnable = (quint8)buf[3];
        pdat->alarmPara.yanWuAlarmEnable = (quint8)buf[4];
        pdat->alarmPara.boLiAlarmEnable = (quint8)buf[5];
        pdat->alarmPara.shuiQinAlarmEnable = (quint8)buf[6];
        pdat->alarmPara.tempAlarmEnable = (quint8)buf[7];
        pdat->alarmPara.doorCiAlarmEnable = (quint8)buf[8];
        pdat->alarmPara.existManAlarmEnable = (quint8)buf[9];
    }
    else
    {
        retValue=1;
    }

    return retValue;
}
/*
 * 读取报警状态
*/
quint8 Func_tcp_75(char *buf,paraData *pdat)
{
    quint8 retValue=0;
    QLOG_INFO()<<"read alarmStatus text return";

    quint8 crc = getCRC((uchar *)buf,15);
    if(crc==(uchar)buf[15])
    {
        pdat->slaveStatus.buttonAlarmStatus = (bool)buf[1];
        pdat->slaveStatus.cutAlarmStatus = (bool)buf[2];
        pdat->slaveStatus.zhengDongAlarmStatus = (bool)buf[3];
        pdat->slaveStatus.yanWuAlarmStatus = (bool)buf[4];
        pdat->slaveStatus.boLiAlarmStatus = (bool)buf[5];
        pdat->slaveStatus.shuiQinAlarmStatus = (bool)buf[6];
        pdat->slaveStatus.tempAlarmStatus = (bool)buf[7];
        pdat->slaveStatus.doorCiAlarmStatus = (bool)buf[8];
        pdat->slaveStatus.existManAlarmStatus = (bool)buf[9];
    }
    else
    {
        retValue=1;
    }

    return retValue;
}
/*
 * 设置时间返回值
*/
quint8 Func_tcp_74(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置报警参数结果返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 清除报警返回值
*/
quint8 Func_tcp_76(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置报警参数结果返回
    uchar crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 加钞间设置状态返回值
*/
quint8 Func_tcp_77(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //设置状态结果返回
    uchar crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 加钞间删除钥匙返回值
*/
quint8 Func_tcp_78(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //删除钥匙结果返回
    uchar crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.comError=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 加钞间读取钥匙返回值
*/
quint8 Func_tcp_79(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    QList<QByteArray> keyList;
    keyList.clear();
    //QLOG_INFO()<<"read led text return";
    quint16 keyNum = (quint16)(buf[2]<<8)|(buf[1]);
    quint16 dataLength = keyNum*16;

    uchar crc = getCRC((uchar *)buf,dataLength+3);
    if(crc==(uchar)buf[dataLength+3])
    {
        //QLOG_INFO()<<"read led text return";

        for(int i=0;i<keyNum;i++)
        {
            QByteArray tempkey;
            tempkey.clear();
            for(int j=0;j<16;j++)
            {
                tempkey.append(buf[(i*16)+j+3]);
            }
            keyList.append(tempkey);
        }
        pdat->keyList = keyList;
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 设备主动上传防切报警解析
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_90(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //防切报警返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.cutAlarmStatus=(quint8)buf[1];
        QString alarm = "切割报警";
        pdat->alarmMsgList.append(alarm);
        //更新日志
        slaveInsetOneLog(pdat,alarm);
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 设备主动上传按钮报警解析
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_91(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //按钮报警返回
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.buttonAlarmStatus=(quint8)buf[1];
        QString alarm = "按钮报警";
        pdat->alarmMsgList.append(alarm);
        //更新日志
        slaveInsetOneLog(pdat,alarm);
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 2.8设备主动上传报警解析
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_92(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    //QLOG_INFO()<<"";

    quint8 crc = getCRC((uchar *)buf,15);
    if(crc==(uchar)buf[15])
    {
        pdat->slaveStatus.buttonAlarmStatus = (quint8)buf[1];
        if(pdat->slaveStatus.buttonAlarmStatus)
        {
            QString alarm = "按钮报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.cutAlarmStatus = (quint8)buf[2];
        if(pdat->slaveStatus.cutAlarmStatus)
        {
            QString alarm = "切割报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.zhengDongAlarmStatus = (quint8)buf[3];
        if(pdat->slaveStatus.zhengDongAlarmStatus)
        {
            QString alarm = "震动报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.yanWuAlarmStatus = (quint8)buf[4];
        if(pdat->slaveStatus.yanWuAlarmStatus)
        {
            QString alarm = "烟雾报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.boLiAlarmStatus = (quint8)buf[5];
        if(pdat->slaveStatus.boLiAlarmStatus)
        {
            QString alarm = "玻璃破碎报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.shuiQinAlarmStatus = (quint8)buf[6];
        if(pdat->slaveStatus.shuiQinAlarmStatus)
        {
            QString alarm = "水浸报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.tempAlarmStatus = (quint8)buf[7];
        if(pdat->slaveStatus.tempAlarmStatus)
        {
            QString alarm = "温度报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.doorCiAlarmStatus = (quint8)buf[8];
        if(pdat->slaveStatus.doorCiAlarmStatus)
        {
            QString alarm = "门磁报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
        pdat->slaveStatus.existManAlarmStatus = (quint8)buf[9];
        if(pdat->slaveStatus.existManAlarmStatus)
        {
            QString alarm = "人存在报警";
            pdat->alarmMsgList.append(alarm);
            //更新日志
            slaveInsetOneLog(pdat,alarm);
        }
    }
    else
    {
        retValue=1;
    }

    return retValue;
}
/*
 * 设备主动上传温度解析
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_99(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    quint16 temperature=0;
    quint16 humidity=0;
    quint8 crc = getCRC((uchar *)buf,7);
    if(crc==(uchar)buf[7])
    {
        temperature = GetTemperature(buf,1);
        humidity = GetHumidity(buf,3);

        pdat->slaveStatus.temperature = temperature;
        pdat->slaveStatus.humidity = humidity;
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 设备主动上传状态数据解析
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 *
 * 设备类型				设备状态
    0：门				0–关；1–开
    1：照明灯				0–关；1–开
    2：灯箱				0–关；1–开
    3：风扇				0–关；1–开
    4：使用中				0–无人；1–有人
    5：启/停用			0–启用；1–停用
    6：锁闭/解锁			0–解锁；1–锁闭

    *****加钞间新增****
    *****
    7：工作状态			0 – 正常状态
                        1 – 管理员状态
                        2 – 注册用户状态
                        3 – 删除用户状态
                        4 – 删除全部用户状态
                        5 – 修改管理员状态
    8：报警（系统）状态
                        0 – 设防状态
                        1 – 等待到撤防状态
                        2 – 撤防状态
                        3 – 等待到设防状态
                        4 – 处于报警状态
*/
quint8 Func_tcp_100(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    quint8 slaveSubModel;//设备类型
    quint8 slaveSubModelStatus;//状态

    uchar crc = getCRC((uchar *)buf,7);

    if(crc!=(uchar)buf[7])return 0;//校验错误 返回

    slaveSubModel = GetSlaveSubModel(buf,1);
    slaveSubModelStatus = GetSlaveSubModelStatus(buf,2);

    switch(slaveSubModel)
    {
    case 0://门
    {
        pdat->slaveStatus.gateStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "舱门打开";
        }
        else
        {
            item = "舱门关闭";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 1://照明灯
    {
        pdat->slaveStatus.zhaomingStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "照明灯 点亮";
        }
        else
        {
            item = "照明灯 熄灭";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 2://灯箱
    {
        pdat->slaveStatus.dengxiangStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "灯箱 点亮";
        }
        else
        {
            item = "灯箱 熄灭";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 3://风扇
    {
        pdat->slaveStatus.fanStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "风扇 启动";
        }
        else
        {
            item = "风扇 停止";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 4://使用中
    {
        pdat->slaveStatus.usedStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "防护舱 使用中";
        }
        else
        {
            item = "防护舱 空闲";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 5://防护舱 启/停用
    {
        pdat->slaveStatus.fanghucangStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "防护舱 停用";
        }
        else
        {
            item = "防护舱 启用";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 6://锁闭/解锁
    {
        pdat->slaveStatus.lockStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "防护舱 锁闭";
        }
        else
        {
            item = "防护舱 解锁";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 7://工作状态
    {

        pdat->slaveStatus.workStatus=slaveSubModelStatus;
        //更新日志
        QString item = "工作状态  ";
        item.append(pdat->slaveStatus.getWorkStatus());

        slaveInsetOneLog(pdat,item);
    }
        break;
    case 8://报警（系统）状态
    {

        pdat->slaveStatus.alarmStatus=slaveSubModelStatus;
        //更新日志
        QString item = "报警状态  ";
        item.append(pdat->slaveStatus.getAlarmStatus());

        slaveInsetOneLog(pdat,item);
    }
        break;
    default:
        retValue=1;
        break;
    }
    return retValue;
}
/*
 * 在有人值守状态下，当读到进出门钥匙数据时，会主动上传到软件
**/
quint8 Func_tcp_101(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    IndoorKeyMsg msg;
    uchar crc = getCRC((uchar *)buf,21);
    if(crc==(uchar)buf[21])
    {
        //提取卡号
        for(int j=0;j<8;j++)
        {
            msg.cardNum[j] = buf[j+1];
        }
        //提取时间
        for(int j=0;j<4;j++)
        {
            msg.time = msg.time<<8;
            msg.time = msg.time |(quint8)buf[12-j];
        }
        //提取类型
        msg.msgEvent = (quint8)buf[13];
        msg.msgType = (quint8)buf[14];
        msg.fushu1 = (quint8)buf[15];
        msg.fushu2 = (quint8)buf[16];
        msg.fushu3 = (quint8)buf[17];

        msg.check = (quint8)buf[20];

        pdat->indoorKeyMsg = msg;

        //更新日志
        QString item = "钥匙号："+msg.getCardNum()+" 位置："+msg.getCardPosition()+" 卡类型："+msg.getCardType();
        slaveInsetOneLog(pdat,item);
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 添加钥匙后返回的钥匙数据
**/
quint8 Func_tcp_102(char *buf, paraData *pdat)
{
    quint8 retValue=0;

    uchar crc = getCRC((uchar *)buf,17);
    if(crc==(uchar)buf[17])
    {
        QByteArray tempkey;
        tempkey.clear();
        for(int i=0;i<16;i++)
        {
            tempkey.append(buf[i+1]);
        }
        pdat->keyList.append(tempkey);
    }
    else
    {
        retValue=1;
    }
    return retValue;
}

/*  加钞间
 * 设备主动上传状态数据解析
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 *
    设备类型				设备状态
    0：门                0  – 关；1 – 开
    1：照明灯            0  – 关；1 – 开
    2：灯箱				0  – 关；1 – 开
    3：风扇				0  – 关；1 – 开
    4：使用中				0  – 无人；1 – 有人
    5：启/停用			0  – 启用；1 – 停用
    6：锁闭/解锁			0  – 解锁；1  – 锁闭

    7：工作状态			0 – 正常状态
                        1 – 管理员状态
                        2 – 注册用户状态
                        3 – 删除用户状态
                        4 – 删除全部用户状态
                        5 – 修改管理员状态
    8：报警（系统）状态
                        0 – 设防状态
                        1 – 等待到撤防状态
                        2 – 撤防状态
                        3 – 等待到设防状态
                        4 – 处于报警状态

*/
quint8 Func_tcp_200(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    quint8 slaveSubModel;//设备类型
    quint8 slaveSubModelStatus;//状态

    uchar crc = getCRC((uchar *)buf,7);

    if(crc!=(uchar)buf[7])return 0;//校验错误 返回

    slaveSubModel = GetSlaveSubModel(buf,1);
    slaveSubModelStatus = GetSlaveSubModelStatus(buf,2);

    switch(slaveSubModel)
    {
    case 0://门
    {
        pdat->slaveStatus.gateStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "舱门打开";
        }
        else
        {
            item = "舱门关闭";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 1://照明灯
    {
        pdat->slaveStatus.zhaomingStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "照明灯 点亮";
        }
        else
        {
            item = "照明灯 熄灭";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 2://灯箱
    {
        pdat->slaveStatus.dengxiangStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "灯箱 点亮";
        }
        else
        {
            item = "灯箱 熄灭";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 3://风扇
    {
        pdat->slaveStatus.fanStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "风扇 启动";
        }
        else
        {
            item = "风扇 停止";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 4://使用中
    {
        pdat->slaveStatus.usedStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "防护舱 使用中";
        }
        else
        {
            item = "防护舱 空闲";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 5://防护舱 启/停用
    {
        pdat->slaveStatus.fanghucangStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "防护舱 停用";
        }
        else
        {
            item = "防护舱 启用";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    case 6://锁闭/解锁
    {
        pdat->slaveStatus.lockStatus=slaveSubModelStatus;
        //更新日志
        QString item;
        if(slaveSubModelStatus)
        {
            item = "防护舱 锁闭";
        }
        else
        {
            item = "防护舱 解锁";
        }
        slaveInsetOneLog(pdat,item);
    }
        break;
    default:
        retValue=1;
        break;
    }
    return retValue;
}
/*
 * 输入：
 * @func 功能码
 * @sendData 发送的数据
 * @length 数据个数
 * 输出：
 * @datagram 要发送的数据包
*/
quint8 createTcpRequest(quint8 func,uchar *sendData,quint16 length,QByteArray *datagram)
{
    QDataStream out(datagram,QIODevice::WriteOnly);
    uchar header=func;
    quint16 dataCount=0;
    uchar D[100]={0};
    uchar crc=0;

    dataCount=length;
    //写入包头
    out<<header;
    //写入数据
    for(int i=0;i<dataCount;i++)
    {
        out<<sendData[i];
    }
    crc = getCRC((uchar *)datagram->data(),dataCount+1);
    out<<crc;

    return 1;
}
quint8 Func_tcp_set_sound(char *buf, paraData *pdat)
{
    QLOG_INFO()<<"set sound return";
    quint8 retValue=0;
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.buttonAlarmStatus=(quint8)buf[1];
        if(pdat->slaveStatus.buttonAlarmStatus)
        {
            QLOG_INFO()<<"写入音乐文件出错！";
        }
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
quint8 Func_tcp_read_sound(char *buf, paraData *pdat)
{
    QLOG_INFO()<<"read sound return";
    quint8 retValue=0;
    TcpFuncCode function = (TcpFuncCode)((quint8)buf[0]-KEY);
    //QLOG_INFO()<<"function:"<<(TcpFuncCode)function;
    quint8 sizeH = (quint8)buf[6];
    quint8 sizeL = (quint8)buf[5];
    quint16 blockSize = ((sizeH<<8)|sizeL);

    quint32 sum = CheckSum32((uchar *)&buf[1],blockSize+7);
    quint32 checksum = (quint8)buf[blockSize+11];
    checksum = (checksum<<8) | (quint8)buf[blockSize+10];
    checksum = (checksum<<8) | (quint8)buf[blockSize+9];
    checksum = (checksum<<8) | (quint8)buf[blockSize+8];
    if(sum==checksum)
    {

        for(int i=0;i<blockSize;i++)
        {
            pdat->mp3File.append((quint8)buf[i+7]);
        }
        //总包个数
        quint8 countH = (quint8)buf[2];
        quint8 countL = (quint8)buf[1];
        quint16 count = ((countH<<8)|countL);
        pdat->soundSize=count;
        QLOG_INFO()<<"总包个数:"<<count;
        //当前包的序号
        quint8 currentH = (quint8)buf[4];
        quint8 currentL = (quint8)buf[3];
        quint16 current = ((currentH<<8)|currentL);
        pdat->readSoundSize = current;
        QLOG_INFO()<<"当前包的序号:"<<current;
        if((count-current)<=1)
        {
            pdat->readSoundDone = 1;
        }

        switch(function)
        {
        case func_tcp_read_hy_sound://

            break;
        case func_tcp_read_syz_sound://

            break;
        case func_tcp_read_whz_sound://

            break;
        case func_tcp_read_timeoutWarn_sound://

            break;
        case func_tcp_read_timeoutTiShi_sound://

            break;
        case func_tcp_read_outDoor_sound://

            break;
        case func_tcp_read_openDoor_sound:

            break;
        case func_tcp_read_failLock_sound:

            break;
        case func_tcp_read_laDoor_sound:

            break;
        }

    }
    else
    {
        retValue=1;
    }
    pdat->resetSoundFlag();
    return retValue;
}
/*
 * 读取声音md5校验值 （返回报文）
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_read_sound_md5(char *buf, paraData *pdat)
{
    QLOG_INFO()<<"read md5 return";
    quint8 retValue=0;
    QByteArray tempMd5;
    TcpFuncCode function = (TcpFuncCode)((quint8)buf[0]-KEY);
    //QLOG_INFO()<<"function:"<<(TcpFuncCode)function;
    quint8 crc = getCRC((uchar *)buf,19);
    if(crc==(uchar)buf[19])
    {

        for(int i=0;i<16;i++)
        {
            tempMd5.append(buf[i+1]);
        }
        //QLOG_INFO()<<"md5:"<<tempMd5.toHex();
        switch(function)
        {
        case func_tcp_read_hy_md5://
            pdat->soundMd5[0] = tempMd5;
            //QLOG_INFO()<<"md50:"<<tempMd5.toHex();
            break;
        case func_tcp_read_syz_md5://
            pdat->soundMd5[1] = tempMd5;
            //QLOG_INFO()<<"md51:"<<tempMd5.toHex();
            break;
        case func_tcp_read_whz_md5://
            pdat->soundMd5[2] = tempMd5;
            //QLOG_INFO()<<"md52:"<<tempMd5.toHex();
            break;
        case func_tcp_read_timeoutWarn_md5://
            pdat->soundMd5[3] = tempMd5;
            //QLOG_INFO()<<"md53:"<<tempMd5.toHex();
            break;
        case func_tcp_read_timeoutTiShi_md5://
            pdat->soundMd5[4] = tempMd5;
            //QLOG_INFO()<<"md54:"<<tempMd5.toHex();
            break;
        case func_tcp_read_outDoor_md5://
            pdat->soundMd5[5] = tempMd5;
            //QLOG_INFO()<<"md55:"<<tempMd5.toHex();
            break;
        case func_tcp_read_openDoor_md5:
            pdat->soundMd5[6] = tempMd5;
            //QLOG_INFO()<<"md56:"<<tempMd5.toHex();
            break;
        case func_tcp_read_failLock_md5:
            pdat->soundMd5[7] = tempMd5;
            //QLOG_INFO()<<"md57:"<<tempMd5.toHex();
            break;
        case func_tcp_read_laDoor_md5:
            pdat->soundMd5[8] = tempMd5;
            //QLOG_INFO()<<"md58:"<<tempMd5.toHex();
            break;
        }
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 设置声音md5校验值 （返回报文）
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_set_sound_md5(char *buf, paraData *pdat)
{
    QLOG_INFO()<<"set md5 return";
    quint8 retValue=0;
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        QLOG_INFO()<<"set sound MD5 return！";
        pdat->slaveStatus.buttonAlarmStatus=(quint8)buf[1];
        if(pdat->slaveStatus.buttonAlarmStatus)
        {
            QLOG_INFO()<<"设置声音MD5出错！";
        }
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 读取声音音量 （返回报文）
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_read_sound_volume(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    quint8 volume=0;
    quint8 crc = getCRC((uchar *)buf,3);
    qDebug()<<(quint8)buf[0];
    if(crc==(uchar)buf[3])
    {
        volume = (quint8)buf[1];
        pdat->soundVolume = volume;
        //QLOG_INFO()<<"readSoundValue:"<<volume;
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
/*
 * 设置声音音量 （返回报文）
 * @buf 从缓冲区中读取的数据
 * @pdat 传递到设备实例中的参数数据
 */
quint8 Func_tcp_set_sound_volume(char *buf, paraData *pdat)
{
    quint8 retValue=0;
    quint8 crc = getCRC((uchar *)buf,3);
    if(crc==(uchar)buf[3])
    {
        pdat->slaveStatus.buttonAlarmStatus=(quint8)buf[1];
    }
    else
    {
        retValue=1;
    }
    return retValue;
}
quint8 createTcpSetVoiceRequest(quint8 func,uchar *sendData,quint16 length,QByteArray *datagram)
{
    QDataStream out(datagram,QIODevice::WriteOnly);
    uchar header=func;
    quint16 dataCount=0;
    quint32 sumCheck=0;

    dataCount=length;

    sumCheck = CheckSum32(sendData,length);
    //写入包头
    out<<header;
    //写入数据
    for(int i=0;i<dataCount;i++)
    {
        out<<sendData[i];
    }

    quint32 sum = sumCheck&0xff;
    sum = (sum<<8) | (sumCheck>>8)&0xff;
    sum = (sum<<8) | (sumCheck>>16)&0xff;
    sum = (sum<<8) | (sumCheck>>24)&0xff;
    out<<sum;

    return 1;
}

