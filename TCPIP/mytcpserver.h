#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include "myclientsocket.h"

class myTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit myTcpServer(QObject *parent = 0);
private:

protected:
    void incomingConnection(int handle);
    
signals:
    void ClientConnect(int clientID,QTcpSocket *client);
private slots:


public slots:
    
};

#endif // MYTCPSERVER_H
