#ifndef PARADATA_H
#define PARADATA_H
/*
 *设备参数类
 * 包括状态参数类，网络参数类，照明参数类，防护舱类
 *
 *
*/
#include <QMutex>
#include <qglobal.h>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"
#include <QDateTime>
#include <QString>
#include "../DATA/equstatus.h"
#include "../DATA/parazhaoming.h"
#include "../DATA/parafanghucang.h"
#include "../DATA/paranetwork.h"
#include "../DATA/slavemsg.h"
#include "../DATA/paraalarm.h"
#include "../DATA/fanghucanglogitem.h"
#include "../DATA/indoorkeymsg.h"
class paraData
{
public:
    paraData();
    ~paraData();
    QString name;       //设备名称
    quint8 slaveType;  //设备模型 0-智能门禁 ff-空 1－加钞间控制器
    QDate slaveDate;    //设备时间
    QTime slaveTime;
    QString version;    //设备版本
    quint8 mainVer;
    quint8 secondVer;
    quint8 twoMainVer;
    quint8 xiuDingVer;
    quint8 waiSheModel;
    quint32 iPreMsgCount;//上一次返回的记录条数
    quint8 dataChanged;//数据发生变化
    QByteArray ledArray[3];
    QString ledText[3];
    QByteArray soundMd5[9];
    QString soundName[9];
    QString voiceFileName[9];
    quint8 soundVolume;
    quint8 readSoundDone;
    quint16 soundSize;
    quint16 readSoundSize;
    QByteArray mp3File;
    QList<QByteArray> keyList;//控制器用户列表
    IndoorKeyMsg indoorKeyMsg;
    //QString xiuDingVer;//修订版本
public:
    equStatus slaveStatus;      //状态类
    paraNetwork  net;           //网络参数类
    paraZhaoMing zhaoMing;      //照明参数类
    paraFangHuCang fangHuCang;  //防护舱参数类
    paraAlarm alarmPara;
    QList<SlaveMsg*> slaveMsg; //设备日志
    QList<SlaveMsg*> emitslaveMsg; //设备单次接收的日志
    QString getSoundMd5(quint8 id);
    QList<QString> alarmMsgList;
    //临时日志
    QList<FangHuCangLogItem*> tempFangHuCangLogList;
    int readedKey;
public:
    //获取设备类型
    QString getSlaveType();
    void setFlag();
    void resetFlag();
    quint8 getFlag();
    quint8 getTwoMainVer();
    void iGetMsgCountAdd(quint32 msgcount);
    quint32 iGetMsgCountCurrentValue();
    void setMsgCount(quint32 msgcount);
    quint32 GetMsgCount();
    void cleariGetMsgCount();
    void setSoundFlag();
    void resetSoundFlag();
    quint8 getSoundFlag();
private:
    quint32 iMsgCount;  //设备的记录总条数
    quint32 iGetMsgCount;//已经读到的记录总数
    quint8 flag;//读信息记录时的标志
    quint8 readSoundFlag;
};

#endif // PARADATA_H
