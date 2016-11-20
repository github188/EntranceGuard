#ifndef PARANETWORK_H
#define PARANETWORK_H
#include <qglobal.h>
#include <QString>
class paraNetwork
{
public:
    paraNetwork();
    ~paraNetwork();

    QString ip;
    QString gateWay;
    QString macAddr;
    QString subnetMask;
    quint8 networkModel;//连网方式 0－服务器 1-客户端
    QString remoteIp;
    quint16 tcpPort;
    quint16 udpPort;
    quint8 dataChanged;//数据发生变化
};

#endif // PARANETWORK_H
