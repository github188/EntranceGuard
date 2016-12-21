#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <QObject>
#include "../DATA/dataopt.h"
#include "../datastruct.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTcpServer>
#include "../CycleBuffer/CCycleBuffer.h"
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"
#include <QTime>
#include <QDateTime>
#include <QTimer>
#include "../DATA/systemlogitem.h"
#include "../DATA/fanghucanglogitem.h"
#include "../TCPIP/mytcpserver.h"
class Equipment : public QObject
{
    Q_OBJECT
public:
    explicit Equipment(QObject *parent = 0);
    ~Equipment();
    void setEquQTcpSocket(QTcpSocket *tcpClient);
    void EquInit(paraData par);//初始化设备类
    quint8 selectStatus;//0-未选种 1－选种
    paraData equParaData;
    bool autoLink;//是否自动连接
    quint32 indexInTree;//在树结构中的id;
    QString parentName;//父的名称
    quint32 parentIndexInTree;//父在树结构中的id
    QList<QString> slaveAlarmList;
    paraData* GetEquAll(void);   //获取设备所有参数
    QString GetEquName(void);   //获取设备名称
    QString GetEquNameMac(void);   //获取设备名称
    QString GetEquIp(void);     //获取设备IP
    QString GetEquMac(void);     //获取设备MAC
    QString GetEquSubnetMask(void); //获取设备子网掩码
    QString GetEquVersion(void);
    QString GetEquModel();
    quint8 GetSlaveType();
    void slotreadSlavePara();
    void emitRefreshFormAndStatusChangedSignals();
    void setZhaoMingPara(paraZhaoMing *par);
    void setWorkPara(paraFangHuCang *par);
signals:
    //接收设备新数据
    void dataUpdate();//内部 数据更新信号
    void sigDataUpdateToForm(Equipment *temp);
    void sigDataUpdateToDataBase(Equipment *temp);
    void sigUpdateTSlave(Equipment *temp);
    void statusChanged();
    void sigAnalysisData();
    void sigSendRequest();
    void sigOpenDoorDone(quint8 ret);
    void sigCloseLockRet(quint8 ret,quint8 result);
    void sigRefreshTime(paraData *temp);//刷新时间
    void sigRefreshStatus(paraData *temp);//刷新状态
    void sigRefreshZhaoMingPara(paraData *temp);//刷新照明参数
    void sigRefreshFangHuCangPara(paraData *temp);
    void sigRefreshSlavePara(paraData *temp);
    void sigStatusChanged(void);
    void sigSlaveReadedMsg(QList<SlaveMsg *> msglist);
    void sigRefreshLedPara(paraData *temp);
    void sigRefreshVoicePara(paraData *temp);
    void sigSaveSound(paraData *temp);
    void sigSoundSize(quint64 size);
    void sigUseModule(quint64);
    void sigReadSoundSize(quint64 size);
    void sigUseModuleSize(quint64 size);
    void closeWindows();
    void sigSetWindowsTitle(QString text);
    void sigChangeSoundFromModule(paraData *temp,quint8 id);
    void sigSlaveWarm(Equipment* slave);
    void sigRefreshAlarmPara(paraData *temp);
    void sigInsertFangHuCangLog(QList<FangHuCangLogItem*> list);
    void sigAddKey(QByteArray key);
    void sigShowIndoorWindow();//在有人值守状态下，当读到进出门钥匙数据时，会主动上传到软件显示窗口
public slots:
    void slotEanbleOrDisableSlave();
    void slotRemoteOpenDoor();
    void slotRemoteCloseLock();
    void slotReadMsgCount();
    void slotReadMsg(quint8 preMsgCount, quint8 model);
    void slotSendRequest();
    void slotReadSlaveMsg();

    void slotReadLed(quint8 dula);
    void slotSetLed(quint8 dula,QString text);
    void slotReadHyVolume();
    void slotSetHyVolume(quint8 volume);
    void slotReadAllSoundMd5();
    void slotSetMd5(quint8 id,QString md5);
    void slotSetSound(quint8 id,QString fileName);
    /*读取声音*/
    void slotReadHySound();
    void slotReadSyzSound();
    void slotReadWhzSound();
    void slotReadTimeoutWarnSound();
    void slotReadTimeoutTiShiSound();
    void slotReadOutDoorSound();
    void slotReadOpenDoorSound();
    void slotReadFailLockSound();
    void slotReadLaDoorSound();
    void slotChangeSlaveSound(quint8 id,QString fileName,QString md5);
    void slotPlaySound(quint8 id);
    void slotUseModule(paraData *temp);
    void slotSetAlarmPara(paraAlarm* alarmPara);
    void slotGetAlarmPara();
    void slotGetAlarmStatus();
    void slotClearAlarm();
    //返回正常状态状态
    void slotReturnNormalStatus();
    //进入增加钥匙状态
    void slotSetAddKeyStatus();
    //进入增加主钥匙状态
    void slotSetAddMainKeyStatus();
    //删除钥匙
    void slotDeleteKey(int id);
    //读钥匙
    void slotReadKey();
    //写入主钥匙
    void slotWriteMainKey(uchar keyData[16]);
    void slotReadSlaveTime();//读取设备时间
    void slotSetSlaveAlarmStatus(quint8 status);
private slots:
    void connectToServer();
    void connectionClosedByServer();
    void connectedToServer();
    void receiveData();
    void error();
    //通信
    void slotAnalysisData();//从缓冲区中请取数据并分析

    void slotSetSlaveTime(quint8 year, quint8 month, quint8 day, quint8 hour, quint8 minute, quint8 second, quint8 week);//读取设备时间
    void slotReadSlaveStatus();//读取设备状态
    void reConnect();
    void slotSetLineAndAdLinePara(paraZhaoMing *par);
    void slotGetLineAndAdLinePara();
    void slotSetWorkPara(paraFangHuCang *par);
    void slotGetWorkPara();
    void slotAutoAddTimeSec();
private:
    QTimer *readTimeTimer;//定时读取时间定时器
    QTimer *detectDisconnectTimer;//检测断线定时器
    QTimer *reConnectTimer;//断线后重连延时定时器
    QTimer *slaveTimeSet;
    quint8 equEnable;//设备是否启动
    quint8 useModule;//设备正在应用模板
    static quint16 tcpPort;//tcp连接端口
    static quint16 udpPort;//udp连接端口
    static quint64 detectDisconnectTime;//心跳包超时时间
    static quint64 reConnectTime;//延时重新连接
    static quint32 readDelayTime;//读取时间间隔，类似心跳包。不能比detectDisconnectTime大
    static quint64 Addsecond;
    int i;
    int usemoduleModel;//应用模板模式，下载语音后不关闭
    CCycleBuffer* comBuffer;
    CCycleBuffer* sendDataBuffer;
    void writeDataToBuffer(QByteArray *datagram);
    quint64 setVoiceData(TcpFuncCode func,QString fileName);
    void setVoiceStart(TcpFuncCode func);
    void setVoiceEnd(TcpFuncCode func,quint32 dataLength);
    void readSound(TcpFuncCode func, quint8 status);
    void ReadSoundVolume(TcpFuncCode func);
    void SetSoundVolume(TcpFuncCode func,quint8 volume);
    void SetSoundMd5(TcpFuncCode func,QByteArray md5);
    void setSound(TcpFuncCode func,QString filename);
    void ReadSoundMd5(TcpFuncCode func);
    void ReadSound(TcpFuncCode func);
    void PlaySound(TcpFuncCode func,quint8 id);

private:
    QTcpSocket *tcpClient;

    //QUdpSocket *udpClient;
    quint16 nextBlockSize;
    quint16 blockSize;
    QString equNameMac;

private:
    void InitEquPara();
    QString equName;
    quint8 header;
    quint8 crcCheck;
    quint8 SetVoiceStage;
    quint8 controlFlag;

};

#endif // EQUIPMENT_H
