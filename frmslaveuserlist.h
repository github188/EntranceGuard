#ifndef FRMSLAVEUSERLIST_H
#define FRMSLAVEUSERLIST_H

#include <QDialog>
#include <QTcpSocket>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"

namespace Ui {
class FrmSlaveUserList;
}

class FrmSlaveUserList : public QDialog
{
    Q_OBJECT

public:
    explicit FrmSlaveUserList(QWidget *parent = 0);
    ~FrmSlaveUserList();
    void setModel(quint8 model);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmSlaveUserList *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    bool bregistered;
private:
    void InitStyle();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnClose();
    void btnDelete();
    void btnReturnNormalStatus();
public slots:
    void DisplayKeyList(QList<QByteArray> keyList);
    void slotAddKey(QByteArray key);
signals:
    void sigDeleteKeyId(int id);
    void sigReturnNormalStatus();
    //void sigRegisterSlave(paraData*);
};

#endif // FRMSLAVEUSERLIST_H
