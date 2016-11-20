#ifndef FRMEDITUSERPWD_H
#define FRMEDITUSERPWD_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "./User/userinfo.h"
namespace Ui {
class FrmEditUserPwd;
}

class FrmEditUserPwd : public QDialog
{
    Q_OBJECT

public:
    explicit FrmEditUserPwd(QWidget *parent = 0);
    ~FrmEditUserPwd();
    void setCurrentUser(UserInfo *user);
    void InitDisplay();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmEditUserPwd *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    UserInfo *currentUser;
    void InitStyle();
signals:
    void sigUpdateUserpwd();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnOK();
    void btnCancel();
};

#endif // FRMEDITUSERPWD_H
