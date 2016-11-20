#ifndef PARAZHAOMING_H
#define PARAZHAOMING_H
#include <qglobal.h>
#include <QTime>
class paraZhaoMing
{
public:
    paraZhaoMing();
    ~paraZhaoMing();
    quint8 Model;//开启方式：0 – 停用；1 - 光控；2 – 时控；3 – 光控+时控
    quint8 startHour;
    quint8 startMinute;
    quint8 endHour;
    quint8 endMinute;
    quint8 manEnable;//有人灯亮
    quint8 dataChanged;//数据发生变化
    QTime getStartTime(void);
    QTime getStopTime(void);
    QString getModel(void);
};

#endif // PARAZHAOMING_H
