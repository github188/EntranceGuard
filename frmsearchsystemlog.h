#ifndef FRMSEARCHSYSTEMLOG_H
#define FRMSEARCHSYSTEMLOG_H

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
class frmSearchSystemLog;
}

class frmSearchSystemLog : public QDialog
{
    Q_OBJECT

public:
    explicit frmSearchSystemLog(QWidget *parent = 0);
    ~frmSearchSystemLog();
    void setDateTime(QDateTime datetime);
    void setDatebaseQuery(QSqlQuery *tempquery );
    void setTitle(const QString &title);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmSearchSystemLog *ui;
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

#endif // FRMSEARCHSYSTEMLOG_H
