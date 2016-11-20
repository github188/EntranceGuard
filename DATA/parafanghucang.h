#ifndef PARAFANGHUCANG_H
#define PARAFANGHUCANG_H
#include <qglobal.h>

class paraFangHuCang
{
public:
    paraFangHuCang();
    ~paraFangHuCang();

    quint8 openLockTime;        //D1：开锁时间（0-30秒钟）
    quint8 OptBussinessTime;    //D2：办理业务时间（无限制，1-50分钟）
    quint8 timeOutRemind;     //D3：超时提醒时间（0-200秒钟）
    quint8 warnningDelayTime;   //D4：提醒间隔时间（0-200秒钟）
    quint16 noManOpenLockTime;  //D5：无人开锁时间（0-800秒钟，10秒递增）
    quint8 fanRunTemperature;   //D7：风扇启动温度（1-50度）
    quint8 signalModel;         //D8：人体信号输入方式（0 - 常闭；1 - 常开）
    quint8 peopleEquModel;      //D9: 人体设备类型（0 – 主动；1 – 被动）
    quint8 lockModel;           //D10: 锁具类型（0-灵性锁；1-电磁锁；2-电插锁；3-电控锁）
    quint8 kongCangLockorNot;   //D11: 空舱是否上锁（0 – 否；1 – 是）
    quint8 fangQiewarnning;     //D12：防切割报警功能（0-不启用；1-启用）
    quint8 dataChanged;//数据发生变化

    /*********加钞间参数*********/
                                //D1 开锁时间（1-10秒钟）
    quint8 userNum;             //D2 刷卡人数（1-10人）N+1，如果设置为0表示单卡进出，不为0表示多卡
    quint8 systemAlarmStatus;   //D3 设置系统报警状态：0 - 撤防；1 - 设防
    quint8 setGuardDelayTime;   //D4 设防延时时间（1-90秒钟）
    quint8 isMonitorOrNot;      //D5 是否有人值守：0 - 无人值守；1 - 有人值守
    quint8 inDoorModel;         //D6: 进门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码
    quint8 outDoorModel;        //D7: 出门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码；5-按钮
                                //D8: 备用0
                                //D9: 备用0
                                //D10: 锁具类型 （0-灵性锁；1-电磁锁；2-电插锁；3-电控锁）
                                //D11: 备用0
                                //D12: 备用0（防切割）
                                //D13: 备用0
                                //D14: 备用0

public:
    QString getlockModel();
    QString getpeopleEquModel();
    QString getInDoorModel();
    QString getOutDoorModel();
};

#endif // PARAFANGHUCANG_H
