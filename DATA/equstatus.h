#ifndef EQUSTATUS_H
#define EQUSTATUS_H

#include <qglobal.h>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"
#include <QDateTime>

class equStatus
{
public:
    equStatus();
    ~equStatus();
    QString getWorkStatus(void);
    QString getAlarmStatus(void);
    quint8 connectStatus;//设备的连接状态 0断开 1连接 2正在连接
    quint16 temperature;//温度
    quint16 humidity;//湿度
    quint8 gateStatus;//门状态
    quint8 zhaomingStatus;//照明
    quint8 advertisementLampStatus;//广告灯
    quint8 dengxiangStatus;//灯箱
    quint8 fanStatus;//风扇状态

    quint8 usedStatus;//使用状态
    quint8 fanghucangStatus;//防护舱状态
    quint8 enableOrNot;//启/停用
    quint8 lockStatus;//闭锁状态

    quint8 comError;//通信错误返回值
    quint8 dataChanged;//数据发生变化
    //报警状态
    bool buttonAlarmStatus;//按钮报警状态
    bool cutAlarmStatus;//切割报警状态
    bool zhengDongAlarmStatus;  //震动报警状态
    bool yanWuAlarmStatus;      //烟雾报警状态
    bool boLiAlarmStatus;       //玻璃破碎报警状态
    bool shuiQinAlarmStatus;    //水浸报警状态
    bool tempAlarmStatus;       //温度报警状态
    bool doorCiAlarmStatus;     //门磁报警状态 非法开门状态
    bool existManAlarmStatus;   //人存在报警状态 非法入侵报警
    /***加钞间新加状态****/
 /*	0 – 正常状态
    1 – 管理员状态
    2 – 注册用户状态
    3 – 删除用户状态
    4 – 删除全部用户状态
    5 – 修改管理员状态
*/
    quint8 workStatus;//工作状态

/*	0 – 设防状态
    1 – 等待到撤防状态
    2 – 撤防状态
    3 – 等待到设防状态
    4 – 处于报警状态
*/
    quint8 alarmStatus;//报警状态
};

#endif // EQUSTATUS_H
