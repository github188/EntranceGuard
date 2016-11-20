#ifndef FRMADDSLAVE_H
#define FRMADDSLAVE_H

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
class frmAddSlave;
}

class frmAddSlave : public QDialog
{
    Q_OBJECT

public:
    explicit frmAddSlave(QWidget *parent = 0);
    ~frmAddSlave();   
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmAddSlave *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    paraData *searchedPara;
    bool bregistered;
private:
    void InitStyle();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void RegisterSlave();
    void btnConnect();
    void btnClose();
public slots:
    void DisplayResult(bool registered, paraData*);
signals:
    void sigConnectToSlave(QString);
    void sigRegisterSlave(paraData*);
};

#endif // FRMADDSLAVE_H
