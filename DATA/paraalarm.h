#ifndef PARAALARM_H
#define PARAALARM_H


class paraAlarm
{
public:
    paraAlarm();
    ~paraAlarm();

    //是否启用报警2.8版本和加钞间
    bool btnAlarmEnable;        //D1：按钮报警
    bool cutAlarmEnable;        //D2：防切割报警使能
    bool zhengDongAlarmEnable;  //D3：震动报警使能
    bool yanWuAlarmEnable;      //D4：烟雾报警使能
    bool boLiAlarmEnable;       //D5：：玻璃破碎报警使能
    bool shuiQinAlarmEnable;    //D6：水浸报警使能
    bool tempAlarmEnable;       //D7：温度报警使能
    //加钞间新增报警
    bool doorCiAlarmEnable;     //D8：门磁报警使能
    bool existManAlarmEnable;   //D9：人存在报警使能

};

#endif // PARAALARM_H
