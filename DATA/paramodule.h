#ifndef PARAMODULE_H
#define PARAMODULE_H

#include <QObject>
#include "../DATA/paradata.h"
class paraModule
{

public:
    paraModule();
    ~paraModule();

    QString name;
    quint8 level;//1:加钞间 2：防护舱
    paraData pdat;
};

#endif // PARAMODULE_H
