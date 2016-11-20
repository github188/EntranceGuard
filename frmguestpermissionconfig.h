#ifndef FRMGUESTPERMISSIONCONFIG_H
#define FRMGUESTPERMISSIONCONFIG_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "./User/userpermission.h"
namespace Ui {
class frmGuestPermissionConfig;
}

class frmGuestPermissionConfig : public QDialog
{
    Q_OBJECT

public:
    explicit frmGuestPermissionConfig(QWidget *parent = 0);
    ~frmGuestPermissionConfig();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmGuestPermissionConfig *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    quint8 model;
    UserPermission user;
    void InitStyle();
signals:
    void sigWriteUserPermitPara(UserPermission *user);
public slots:
    void initPara(UserPermission guest);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnOK();
    void btnCancel();
};

#endif // FRMGUESTPERMISSIONCONFIG_H
