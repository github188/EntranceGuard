#ifndef FRMALARMBOX_H
#define FRMALARMBOX_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../Equipment/equipment.h"
#include <phonon>
namespace Ui {
class frmAlarmBox;
}

class frmAlarmBox : public QDialog
{
    Q_OBJECT

public:
    explicit frmAlarmBox(QWidget *parent = 0);
    ~frmAlarmBox();
    void setSlave(Equipment * slave);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmAlarmBox *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    Phonon::MediaObject *mMediaObj;
    void InitStyle();
    quint8 enable[10];
signals:
    //信号
    void sigClearAlarm();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnOK();
    void finished();
};

#endif // FRMALARMBOX_H
