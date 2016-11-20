#ifndef FRMSOFTWAREINFO_H
#define FRMSOFTWAREINFO_H

#include <QDialog>
#include "iconhelper.h"
#include "../myhelper.h"
namespace Ui {
class frmSoftwareInfo;
}

class frmSoftwareInfo : public QDialog
{
    Q_OBJECT

public:
    explicit frmSoftwareInfo(QWidget *parent = 0);
    ~frmSoftwareInfo();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmSoftwareInfo *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

    void InitStyle();
signals:

private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();

};

#endif // FRMSOFTWAREINFO_H
