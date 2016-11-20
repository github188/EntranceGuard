#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QDialog>
#include "iconhelper.h"
#include "myhelper.h"
#include "./frmmain.h"
namespace Ui {
class FrmLogin;
}

class FrmLogin : public QDialog
{
    Q_OBJECT

public:
    explicit FrmLogin(QWidget *parent = 0);
    ~FrmLogin();
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_btnLogin_clicked(); //用户按下登录按钮
    void on_btnClose_clicked(); //关闭界面
private:
    Ui::FrmLogin *ui;
    frmMain *frm;
    QList<UserInfo> userList;//用户列表
    void InitForm();            //初始化界面
};

#endif // FRMLOGIN_H
