#include "equstatus.h"

equStatus::equStatus()
{
    connectStatus=0;//设备的连接状态 0断开 1连接 2正在连接
    temperature=0;//温度
    humidity=0;//湿度
    gateStatus=0;//门状态
    zhaomingStatus=0;//照明
    dengxiangStatus=0;//灯箱
    fanStatus=0;//风扇状态
    buttonAlarmStatus=0;//按钮报警状态
    usedStatus=0;//使用状态
    fanghucangStatus=0;//防护舱状态
    enableOrNot=0;//启/停用
    lockStatus=0;//闭锁状态
    cutAlarmStatus=0;//切割报警状态
    dataChanged=0;//数据发生变化时为1

    buttonAlarmStatus=0;//按钮报警状态
    cutAlarmStatus=0;//切割报警状态
    zhengDongAlarmStatus=0;  //震动报警使能
    yanWuAlarmStatus=0;      //烟雾报警使能
    boLiAlarmStatus=0;       //玻璃破碎报警使能
    shuiQinAlarmStatus=0;    //水浸报警使能
    tempAlarmStatus=0;       //温度报警使能
    doorCiAlarmStatus=0;     //门磁报警状态 非法开门状态
    existManAlarmStatus=0;   //人存在报警状态 非法入侵报警
    /***加钞间新加状态****/
    workStatus=0;//工作状态
    alarmStatus=0;//报警状态
}
equStatus::~equStatus()
{

}
QString equStatus::getWorkStatus(void)
{
    QString item = "";
    switch(workStatus)
    {
    case 0:
        item.append("正常");
        break;
    case 1:
        item.append("管理员");
        break;
    case 2:
        item.append("注册用户");
        break;
    case 3:
        item.append("删除用户");
        break;
    case 4:
        item.append("删除全部用户");
        break;
    case 5:
        item.append("修改管理员");
        break;
    default:
        break;
    }
    item.append("状态");
    return item;
}
QString equStatus::getAlarmStatus(void)
{
    QString item = "";
    switch(alarmStatus)
    {
    case 0:
        item.append("设防");
        break;
    case 1:
        item.append("等待到撤防");
        break;
    case 2:
        item.append("撤防");
        break;
    case 3:
        item.append("等待到设防");
        break;
    case 4:
        item.append("处于报警");
        break;
    default:
        break;
    }
    item.append("状态");
    return item;
}
