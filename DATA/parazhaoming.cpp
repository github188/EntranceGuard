#include "parazhaoming.h"

paraZhaoMing::paraZhaoMing()
{
    Model = 0;//开启方式：0 – 停用；1 - 光控；2 – 时控；3 – 光控+时控
    startHour = 0;
    startMinute = 0;
    endHour = 0;
    endMinute = 0;
    manEnable = 0;//有人灯亮
    dataChanged=0;//数据发生变化时为1
}
paraZhaoMing::~paraZhaoMing()
{

}
QTime paraZhaoMing::getStartTime(void)
{
    QTime startTime;
    startTime.setHMS(startHour,startMinute,0);
    return startTime;
}
QTime paraZhaoMing::getStopTime(void)
{
    QTime stopTime;
    stopTime.setHMS(endHour,endMinute,0);
    return stopTime;
}
QString paraZhaoMing::getModel()
{
    //quint8 Model;//开启方式：0 – 停用；1 - 光控；2 – 时控；3 – 光控+时控
    QString retValue="";
    switch(Model)
    {
    case 0:
        retValue = "停用";
        break;
    case 1:
        retValue = "光控";
        break;
    case 2:
        retValue = "时控";
        break;
    case 3:
        retValue = "光控+时控";
        break;
    default:break;
    }
    return retValue;
}
