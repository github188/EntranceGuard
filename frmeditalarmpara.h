#ifndef FRMEDITALARMPARA_H
#define FRMEDITALARMPARA_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"

namespace Ui {
class frmEditAlarmPara;
}

class frmEditAlarmPara : public QDialog
{
    Q_OBJECT

public:
    explicit frmEditAlarmPara(QWidget *parent = 0);
    ~frmEditAlarmPara();
    void initPara(paraAlarm *paraTemp, quint8 mod);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmEditAlarmPara *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    paraAlarm para;
    quint8 model;
    void InitStyle();
signals:
    void sigWriteAlarmPara(paraAlarm *par);
    void sigSaveAlarmPara(paraAlarm *par);
public slots:
    void initPara(paraAlarm paraTemp, quint8 mod);
    void setSlaveType(quint8 slaveType);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnEdit();
    void btnCancel();
    void on_rbBtnEnable_clicked();
    void on_rbBtnDisable_clicked();
    void on_rbFangQieEnable_clicked();
    void on_rbFangQieDisable_clicked();
    void on_rbZhenDongEnable_clicked();
    void on_rbZhenDongDisable_clicked();
    void on_rbYanWuEnable_clicked();
    void on_rbYanWuDisable_clicked();
    void on_rbBoLiDisable_clicked();
    void on_rbBoLiEnable_clicked();
    void on_rbShuiQinDisable_clicked();
    void on_rbShuiQinEnable_clicked();
    void on_rbTempDisable_clicked();
    void on_rbTempEnable_clicked();
    void on_rbIllegalOpenDoorEnable_clicked();
    void on_rbIllegalOpenDoorDisable_clicked();
    void on_rbIllegalInEnable_clicked();
    void on_rbIllegalInDisable_clicked();
};

#endif // FRMEDITALARMPARA_H
