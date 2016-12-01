#include "slavemsg.h"

SlaveMsg::SlaveMsg()
{
    time=0;
    msgType=0;
    fushu1=0;
    fushu2=0;
}
SlaveMsg::~SlaveMsg()
{

}
QString SlaveMsg::getCardNum(void)
{
    QString retValue="";

    return retValue;
}
QString SlaveMsg::getTime(void)
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
QString SlaveMsg::getTypeName(void)
{
    QString retValue="";

    switch(msgType)
    {
    case 0:
        break;
    case 1:
        retValue = "【输入信号记录事件】";
        break;
    case 2:
        retValue = "【输出信号记录事件】";
        break;
    case 3:
        retValue = "【舱体使用相关事件】";
        break;
    case 4:
        retValue = "【语音相关事件】";
        break;
    case 5:
        retValue = "【报警相关事件】";
        break;
    case 200:
        retValue = "【系统复位】";
        break;
    case 201:
        retValue = "【系统未定义中断】";
        break;
    default:
        break;
    }
    return retValue;
}
QString SlaveMsg::getFuShu_1_1(void)
{
    QString retValue="";
    switch(fushu1)
    {
    case 0:
        retValue = "门外按钮记录";
        break;
    case 1:
        retValue = "门内按钮记录";
        break;
    case 2:
        retValue = "报警按钮记录";
        break;
    case 3:
        retValue = "人体检测信号记录";
        break;
    case 4:
        retValue = "锁舌信号记录";
        break;
    case 5:
        retValue = "防切割信号记录";
        break;
    case 6:
        retValue = "光检信号记录";
        break;
    case 7:
        retValue = "门磁信号记录";
        break;
    case 8:
        retValue = "停用按钮记录";
        break;
    case 9:
        retValue = "震动记录";
        break;
    case 10:
        retValue = "烟感记录";
        break;
    case 11:
        retValue = "温度记录";
        break;
    case 12:
        retValue = "水浸记录";
        break;
    case 13:
        retValue = "玻璃破碎记录";
        break;
    case 14:
        retValue = "红外记录";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_1_2(void)
{
    QString retValue="";
    switch(fushu1)
    {
    case 0:
        retValue = "灯箱灯输出";
        break;
    case 1:
        retValue = "照明灯输出";
        break;
    case 2:
        retValue = "风扇输出";
        break;
    case 3:
        retValue = "锁具供电输出";
        break;
    case 4:
        retValue = "开锁输出";
        break;
    case 5:
        retValue = "锁死输出";
        break;
    case 6:
        retValue = "报警输出";
        break;
    case 7:
        retValue = "110联动输出";
        break;
    case 8:
        retValue = "监控联动输出";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_1_3(void)
{
    QString retValue="";
    switch(fushu1)
    {
    case 0:
        retValue = "是否允许门外按钮开门";
        break;
    case 1:
        retValue = "使用状态变化";
        break;
    case 2:
        retValue = "使能状态变化";
        break;
    case 3:
        retValue = "锁闭状态变化";
        break;
    case 4:
        retValue = "门外按钮未能开门";
        break;
    case 5:
        retValue = "门内按钮未能开门";
        break;
    case 6:
        retValue = "软件开门";
        break;
    case 7:
        retValue = "远程锁闭未能完成";
        break;
    case 8:
        retValue = "";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_1_4(void)
{
    QString retValue="";
    switch(fushu1)
    {
    case 0:
        retValue = "试听";
        break;
    case 1:
        retValue = "播放";
        break;
    case 2:
        retValue = "";
        break;
    case 3:
        retValue = "";
        break;
    case 4:
        retValue = "";
        break;
    case 5:
        retValue = "";
        break;
    case 6:
        retValue = "";
        break;
    case 7:
        retValue = "";
        break;
    case 8:
        retValue = "";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_1_5(void)
{
    QString retValue="";
    switch(fushu1)
    {
    case 0:
        retValue = "按钮报警事件";
        break;
    case 1:
        retValue = "防切割报警事件";
        break;
    case 2:
        retValue = "震动报警事件";
        break;
    case 3:
        retValue = "烟雾报警事件";
        break;
    case 4:
        retValue = "温度报警事件";
        break;
    case 5:
        retValue = "水浸报警事件";
        break;
    case 6:
        retValue = "人体触发报警事件";
        break;
    case 7:
        retValue = "玻璃破碎报警事件";
        break;
    case 8:
        retValue = "强行开门报警事件";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_1_200(void)
{
    QString retValue="";
    switch(fushu1)
    {
    case 0:
        retValue = "";
        break;
    case 1:
        retValue = "系统上电复位记录";
        break;
    case 2:
        retValue = "系统看门狗复位记录";
        break;
    case 3:
        retValue = "系统复位按键复位记录";
        break;
    case 4:
        retValue = "";
        break;
    case 5:
        retValue = "";
        break;
    case 6:
        retValue = "";
        break;
    case 7:
        retValue = "";
        break;
    case 8:
        retValue = "";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_1_201(void)
{
    QString retValue="";
    switch(fushu1)
    {
    case 0:
        retValue = "";
        break;
    case 1:
        retValue = "系统异常中断记录";
        break;
    case 2:
        retValue = "系统异常使用中断记录";
        break;
    case 3:
        retValue = "";
        break;
    case 4:
        retValue = "";
        break;
    case 5:
        retValue = "";
        break;
    case 6:
        retValue = "";
        break;
    case 7:
        retValue = "";
        break;
    case 8:
        retValue = "";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_1_name(void)
{
    QString retValue="";
    switch(msgType)
    {
    case 0:
        break;
    case 1:
        retValue=getFuShu_1_1();
        break;
    case 2:
        retValue=getFuShu_1_2();
        break;
    case 3:
        retValue=getFuShu_1_3();
        break;
    case 4:
        retValue=getFuShu_1_4();
        break;
    case 5:
        retValue=getFuShu_1_5();
        break;
    case 200:
        retValue=getFuShu_1_200();
        break;
    case 201:
        retValue=getFuShu_1_201();
        break;
    default:
        break;
    }
    return retValue;
}
QString SlaveMsg::getFuShu_2_1(void)
{
    QString retValue="";
    /*
    switch(fushu2)
    {
    case 0:
        retValue = "门外按钮记录";
        break;
    case 1:
        retValue = "门内按钮记录";
        break;
    default:
        break;
    }
    */
    return retValue;
}
QString SlaveMsg::getFuShu_2_2(void)
{
    QString retValue="";
    /*
    switch(fushu2)
    {
    case 0:
        retValue = "灯箱灯输出";
        break;
    case 1:
        retValue = "照明灯输出";
        break;
    default:
        break;
    }
    */
    return retValue;
}
QString SlaveMsg::getFuShu_2_3(void)
{
    QString retValue="";
    /*
    switch(fushu2)
    {
    case 0:
        retValue = "是否允许门外按钮开门";
        break;
    case 1:
        retValue = "使用状态变化";
        break;
    default:
        break;
    }
    */
    return retValue;
}
QString SlaveMsg::getFuShu_2_4(void)
{
    QString retValue="";
    switch(fushu2)
    {
    case 0:
        retValue = "";
        break;
    case 1:
        retValue = "进门语音";
        break;
    case 2:
        retValue = "使用中语音";
        break;
    case 3:
        retValue = "维护中语音";
        break;
    case 4:
        retValue = "使用提醒语音";
        break;
    case 5:
        retValue = "使用超时语音";
        break;
    case 6:
        retValue = "出门语音";
        break;
    case 7:
        retValue = "门开提示安全语音";
        break;
    case 8:
        retValue = "门未锁好提示语音";
        break;
    case 9:
        retValue = "请拉门语音";
        break;
    default:
        break;
    }

    return retValue;
}
QString SlaveMsg::getFuShu_2_5(void)
{
    QString retValue="";
    /*
    switch(fushu2)
    {
    case 0:
        retValue = "按钮报警事件";
        break;
    case 1:
        retValue = "防切割报警事件";
        break;
    default:
        break;
    }
    */
    return retValue;
}
QString SlaveMsg::getFuShu_2_200(void)
{
    QString retValue="";
    /*
    switch(fushu2)
    {
    case 0:
        retValue = "";
        break;
    case 1:
        retValue = "系统上电复位记录";
        break;
    default:
        break;
    }
    */
    return retValue;
}
QString SlaveMsg::getFuShu_2_201(void)
{
    QString retValue="";
    /*
    switch(fushu2)
    {
    case 0:
        retValue = "";
        break;
    case 1:
        retValue = "系统异常中断记录";
        break;
    default:
        break;
    }
    */
    return retValue;
}
QString SlaveMsg::getFuShu_2_name(void)
{
    QString retValue="";
    switch(msgType)
    {
    case 0:
        break;
    case 1:
        retValue=getFuShu_2_1();
        break;
    case 2:
        retValue=getFuShu_2_2();
        break;
    case 3:
        retValue=getFuShu_2_3();
        break;
    case 4:
        retValue=getFuShu_2_4();
        break;
    case 5:
        retValue=getFuShu_2_5();
        break;
    case 200:
        retValue=getFuShu_2_200();
        break;
    case 201:
        retValue=getFuShu_2_201();
        break;
    default:
        break;
    }
    return retValue;
}
QString SlaveMsg::getLogItem()
{
    QString item = "记录："+this->getTypeName()+this->getTime()+" "+this->getFuShu_1_name()+" "+this->getFuShu_2_name();
    return item;
}
