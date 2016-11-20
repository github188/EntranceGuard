#include "mytcpserver.h"
//#include <myhelper.h>
#include <QHostAddress>

myTcpServer::myTcpServer(QObject *parent) :
    QTcpServer(parent)
{
}

void myTcpServer::incomingConnection(int handle)
{
    QTcpSocket *client=new QTcpSocket(this);
    client->setSocketDescriptor(handle);
    emit ClientConnect(handle, client);
}

