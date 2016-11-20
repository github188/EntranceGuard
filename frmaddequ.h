#ifndef FRMADDEQU_H
#define FRMADDEQU_H

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
class frmAddEqu;
}

class frmAddEqu : public QDialog
{
    Q_OBJECT

public:
    explicit frmAddEqu(QWidget *parent = 0);
    ~frmAddEqu();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmAddEqu *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

    void InitStyle();
signals:
    //添加设备信号
    void AddEqument(QString ip);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnAddEqument();
    void btnCancel();
};

#endif // FRMADDEQU_H
