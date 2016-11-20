#ifndef FRMEDITZHAOMINGPARA_H
#define FRMEDITZHAOMINGPARA_H

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
class frmEditZhaoMingPara;
}

class frmEditZhaoMingPara : public QDialog
{
    Q_OBJECT

public:
    explicit frmEditZhaoMingPara(QWidget *parent = 0);
    ~frmEditZhaoMingPara();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmEditZhaoMingPara *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    paraZhaoMing para;
    quint8 model;
    void InitStyle();
signals:
    void sigWriteZhaoMingPara(paraZhaoMing *par);
    void sigSaveZhaoMingPara(paraZhaoMing *par);
    void sigSaveAreaZhaoMingPara(paraZhaoMing *par);
public slots:
    void initPara(paraZhaoMing paraTemp,quint8 mod);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnEdit();
    void btnCancel();
    void on_checkboxWhenUsed_clicked(bool checked);
};

#endif // FRMEDITZHAOMINGPARA_H
