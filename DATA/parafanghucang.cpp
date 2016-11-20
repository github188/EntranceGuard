#include "parafanghucang.h"
#include <QString>
paraFangHuCang::paraFangHuCang()
{
    openLockTime=0;        //D1：开锁时间（0-30秒钟）
    OptBussinessTime=0;    //D2：办理业务时间（无限制，1-50分钟）
    timeOutRemind=0;     //D3：超时提醒时间（0-200秒钟）
    warnningDelayTime=0;   //D4：提醒间隔时间（0-200秒钟）
    noManOpenLockTime=0;  //D5：无人开锁时间（0-800秒钟，10秒递增）
    fanRunTemperature=0;   //D7：风扇启动温度（1-50度）
    signalModel=0;         //D8：人体信号输入方式（0 - 常闭；1 - 常开）
    peopleEquModel=0;      //D9: 人体设备类型（0 – 主动；1 – 被动）
    lockModel=0;           //D10: 锁具类型（0-灵性锁；1-电磁锁；2-电插锁；3-电控锁）
    kongCangLockorNot=0;   //D11: 空舱是否上锁（0 – 否；1 – 是）
    fangQiewarnning=0;     //D12：防切割报警功能（0-不启用；1-启用）
    dataChanged=0;//数据发生变化时为1
    QString temp="";

    /*********加钞间参数*********/
                                //D1 开锁时间（1-10秒钟）
    userNum=0;             //D2 刷卡人数（1-10人）N+1，如果设置为0表示单卡进出，不为0表示多卡
    systemAlarmStatus=0;   //D3 设置系统报警状态：0 - 撤防；1 - 设防
    setGuardDelayTime=0;   //D4 设防延时时间（1-90秒钟）
    isMonitorOrNot=0;      //D5 是否有人值守：0 - 无人值守；1 - 有人值守
    inDoorModel=0;         //D6: 进门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码
    outDoorModel=0;        //D7: 出门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码；5-按钮
                        //D8: 备用0
                        //D9: 备用0
                        //D10: 锁具类型 （0-灵性锁；1-电磁锁；2-电插锁；3-电控锁）
                        //D11: 备用0
                        //D12: 备用0（防切割）
                        //D13: 备用0
                        //D14: 备用0
}

QString paraFangHuCang::getlockModel()
{
    //lockModel=0;锁具类型（0-灵性锁；1-电磁锁；2-电插锁；3-电控锁）
    QString retValue="";
    switch(lockModel)
    {
    case 0:
        retValue = "灵性锁";
        break;
    case 1:
        retValue = "电磁锁";
        break;
    case 2:
        retValue = "电插锁";
        break;
    case 3:
        retValue = "电控锁";
        break;
    default:break;
    }
    return retValue;
}
//
QString paraFangHuCang::getInDoorModel()
{
    //进门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码
    QString retValue="";
    switch(inDoorModel)
    {
    case 0:
        retValue = "指纹";
        break;
    case 1:
        retValue = "ID卡";
        break;
    case 2:
        retValue = "TM卡";
        break;
    case 3:
        retValue = "ID卡+密码";
        break;
    case 4:
        retValue = "密码";
        break;
    default:break;
    }
    return retValue;
}
QString paraFangHuCang::getOutDoorModel()
{
    //出门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码；5-按钮
    QString retValue="";
    switch(outDoorModel)
    {
    case 0:
        retValue = "指纹";
        break;
    case 1:
        retValue = "ID卡";
        break;
    case 2:
        retValue = "TM卡";
        break;
    case 3:
        retValue = "ID卡+密码";
        break;
    case 4:
        retValue = "密码";
        break;
    case 5:
        retValue = "按钮";
        break;
    default:break;
    }
    return retValue;
}
paraFangHuCang::~paraFangHuCang()
{

}

