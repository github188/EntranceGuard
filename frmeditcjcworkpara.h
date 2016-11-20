#ifndef FRMEDITCJCWORKPARA_H
#define FRMEDITCJCWORKPARA_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"

namespace Ui {
class FrmEditCJCWorkPara;
}

class FrmEditCJCWorkPara : public QDialog
{
    Q_OBJECT

public:
    explicit FrmEditCJCWorkPara(QWidget *parent = 0);
    ~FrmEditCJCWorkPara();
    void initPara(paraFangHuCang paraTemp, quint8 mod);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmEditCJCWorkPara *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    paraFangHuCang para;
    quint8 model;
    void InitStyle();
signals:
    void sigWriteCjcWorkPara(paraFangHuCang *par);
    //void sigSaveAlarmPara(paraAlarm *par);
public slots:
    //void initPara(paraAlarm paraTemp, quint8 mod);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnEdit();
    void btnCancel();

    void on_rbSetGuard_clicked();
    void on_rbResetGuard_clicked();
    void on_rbExistMan_clicked();
    void on_rbNoMan_clicked();
};

#endif // FRMEDITCJCWORKPARA_H
