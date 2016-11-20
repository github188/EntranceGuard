#include "paraalarm.h"

paraAlarm::paraAlarm()
{
    btnAlarmEnable=0;        //按钮报警
    cutAlarmEnable=0;        //防切割报警使能
    zhengDongAlarmEnable=0;  //震动报警使能
    yanWuAlarmEnable=0;      //烟雾报警使能
    boLiAlarmEnable=0;       //玻璃破碎报警使能
    shuiQinAlarmEnable=0;    //水浸报警使能
    tempAlarmEnable=0;       //温度报警使能
    doorCiAlarmEnable=0;      //非法开门报警
    existManAlarmEnable=0;  //非法入侵报警
}

paraAlarm::~paraAlarm()
{

}
