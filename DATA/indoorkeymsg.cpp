#include "indoorkeymsg.h"

IndoorKeyMsg::IndoorKeyMsg()
{
    time=0;//时间，4个字节
    msgType=0;//记录类型，1个字节
    msgEvent=0;//记录事件，1个字节
    fushu1=0;//附属信息1，1个字节
    fushu2=0;//附属信息2，1个字节
    fushu3=0;//附属信息3，1个字节
    check=0;//校验，1个字节
}
QString IndoorKeyMsg::getCardNum(void)
{
    QString retValue="";

    return retValue;
}
QString IndoorKeyMsg::getTime(void)
{
    QString retValue="";
    int year=0;
    int month=0;
    int day=0;
    int hour=0;
    int minutes=0;
    int second=0;
    second = time%60;
    minutes = (time/60)%60;
    hour = ((time/60)/60)%24;
    day = (((time/60)/60)/24)%31;
    month = ((((time/60)/60)/24)/31)%12;
    year = ((((time/60)/60)/24)/31)/12;
    retValue = "20"+QString::number(year)+"-"+QString::number(month)+"-"+QString::number(day)
            +" "+QString::number(hour)+":"+QString::number(minutes)+":"+QString::number(second);

    return retValue;
}
QString IndoorKeyMsg::getTypeName(void)
{
    QString retValue="";

    switch(msgType)
    {
    case 0:
        break;
    case 6:
        retValue = "【门禁事件】";
        break;
    default:
        break;
    }
    return retValue;
}
QString IndoorKeyMsg::getEventName(void)
{
    QString retValue="";

    switch(msgType)
    {
    case 9:
        retValue = "【刷卡成功】";
        break;
    case 10:
        retValue = "【刷卡失败】";
        break;
    default:
        break;
    }
    return retValue;
}
//获取刷卡位置
quint8 IndoorKeyMsg::getCardPosition(void)//刷卡位置
{
    quint8 retValue=0;
    retValue = fushu1>>4;
    return retValue;
}
//获取刷卡类型
quint8 IndoorKeyMsg::getCardType(void)
{
    quint8 retValue=0;

    retValue = fushu1&0xf0;

    return retValue;
}
//获取刷卡位置
QString IndoorKeyMsg::getCardPositionStr(void)
{
    QString retValue="";

    switch(getCardType())
    {
    case 0x20:
        retValue = "进门位置";
        break;
    case 0x30:
        retValue = "出门位置";
        break;
    default:
        break;
    }

    return retValue;
}
QString IndoorKeyMsg::getFuShu_2(void)
{
    QString retValue="";
    if(msgType == 9)
    {
        switch(fushu2)
        {
        case 0:
            retValue = "没有存储";
            break;
        case 6:
            retValue = "主卡";
            break;
        case 7:
            retValue = "设/撤防卡";
            break;
        case 8:
            retValue = "开门卡";
            break;
        case 9:
            retValue = "多卡开门";
            break;
        default:
            break;
        }
    }
    else if(msgType == 10)
    {

    }

    return retValue;
}
