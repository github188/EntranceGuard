#ifndef SYSTEMLOGITEM_H
#define SYSTEMLOGITEM_H
#include <QDateTime>

class SystemLogItem
{
public:
    SystemLogItem();
    ~SystemLogItem();

    QDateTime dataTime;
    QString LogItem;
};

#endif // SYSTEMLOGITEM_H
