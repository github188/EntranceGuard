#ifndef FRMSEARCHSLAVELOG_H
#define FRMSEARCHSLAVELOG_H

#include <QDialog>
#include <QTcpSocket>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"
#include <QtSql>
namespace Ui {
class frmSearchSlaveLog;
}

class frmSearchSlaveLog : public QDialog
{
    Q_OBJECT

public:
    explicit frmSearchSlaveLog(QWidget *parent = 0);
    ~frmSearchSlaveLog();
    void setSlaveList(QList<TreeViewItem*> viewlist);
    void setDateTime(QDateTime datetime);
    void setDatebaseQuery(QSqlQuery *tempquery );
    void setTitle(const QString &title);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmSearchSlaveLog *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlQuery *query;
private:
    void InitStyle();
    void clearTablewidgetItem();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnSearch();
    void btnExportExcel();
    void btnDeleteLog();
public slots:

signals:

};

#endif // FRMSEARCHSLAVELOG_H
