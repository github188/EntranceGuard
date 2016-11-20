#ifndef FANGHUCANGLOGITEM_H
#define FANGHUCANGLOGITEM_H
#include <QDateTime>

class FangHuCangLogItem
{
public:
    FangHuCangLogItem();
    ~FangHuCangLogItem();
    QDateTime dateTime;
    QString name;
    QString logItem;
};

#endif // FANGHUCANGLOGITEM_H
