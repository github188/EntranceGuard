#ifndef FRMREADSOUNDPROCESS_H
#define FRMREADSOUNDPROCESS_H

#include <QDialog>

namespace Ui {
class FrmReadSoundProcess;
}

class FrmReadSoundProcess : public QDialog
{
    Q_OBJECT

public:
    explicit FrmReadSoundProcess(QWidget *parent = 0);
    ~FrmReadSoundProcess();
    void setProcessMaxValue(quint64 maxValue);
    void setPBarMainMaxValue(quint64 maxValue);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnOk_clicked();
public slots:
    void slotSetWindowsText(QString text);
    void setProcessCurrentValue(quint64 currentValue);
    void setPBarMainCurrentValue(quint64 currentValue);
private:
    Ui::FrmReadSoundProcess *ui;

    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下
};

#endif // FRMREADSOUNDPROCESS_H
