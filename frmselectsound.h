#ifndef FRMSELECTSOUND_H
#define FRMSELECTSOUND_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"
#include <QtSql>
namespace Ui {
class FrmSelectSound;
}

class FrmSelectSound : public QDialog
{
    Q_OBJECT

public:
    explicit FrmSelectSound(QWidget *parent = 0);
    ~FrmSelectSound();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmSelectSound *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlTableModel *tableModel;
    void InitStyle();
signals:
    void sigoperate(QString name);
    void sigPlaySound(QString name);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void on_btnOk_clicked();
    void btnCancel();
    void btnPlaySound();
public slots:
    void setTreeViewModel(QSqlTableModel * model);
};

#endif // FRMSELECTSOUND_H
