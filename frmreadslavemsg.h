#ifndef FRMREADSLAVEMSG_H
#define FRMREADSLAVEMSG_H

#include <QDialog>

namespace Ui {
class frmReadSlaveMsg;
}

class frmReadSlaveMsg : public QDialog
{
    Q_OBJECT

public:
    explicit frmReadSlaveMsg(QWidget *parent = 0);
    ~frmReadSlaveMsg();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnOk_clicked();
public slots:
    void setProcess(quint32 currentValue,quint32 value);
private:
    Ui::frmReadSlaveMsg *ui;

    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下
};

#endif // FRMREADSLAVEMSG_H
