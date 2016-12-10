#ifndef SLAVEVERSION_H
#define SLAVEVERSION_H

#include <QObject>

class SlaveVersion
{
public:
    SlaveVersion();
    QString version;//版本号
    quint8 level; // 版本号等级
    QString name;//模板名称
};

#endif // SLAVEVERSION_H
