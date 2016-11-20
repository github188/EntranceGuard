#ifndef FRMEDITPARAMATER_H
#define FRMEDITPARAMATER_H

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
class frmEditParamater;
}

class frmEditParamater : public QDialog
{
    Q_OBJECT

public:
    explicit frmEditParamater(QWidget *parent = 0);
    ~frmEditParamater();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmEditParamater *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    paraFangHuCang para;
    quint8 model;
    bool editFlag;
    void InitStyle();
signals:
    void sigWriteFangHuCangPara(paraFangHuCang *par);
    void sigSaveFangHuCangPara(paraFangHuCang *par);
    void sigSaveAreaFangHuCangPara(paraFangHuCang *par);
public slots:
    void initPara(paraFangHuCang paraTemp, quint8 mod);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnEdit();
    void btnCancel();
    void on_rBYes_clicked(bool checked);
    void on_rBNo_clicked(bool checked);
    void on_rbEnable_clicked(bool checked);
    void on_rbDisable_clicked(bool checked);
    void on_lefanStartTemperature_editingFinished();
    void on_leWaitTime_editingFinished();
    void on_leOperatTime_editingFinished();
    void on_leRemaindTime_editingFinished();
    void on_leRemaindInterval_editingFinished();
};

#endif // FRMEDITPARAMATER_H
