#include "frmlogin.h"
#include "ui_frmlogin.h"
#include "./frmmain.h"
#include "./AppConfig/myapp.h"
FrmLogin::FrmLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmLogin)
{
    ui->setupUi(this);

    //设置窗体标题栏隐藏
    //this->setWindowFlags(Qt::FramelessWindowHint);
    myHelper::FormInCenter(this);
    myHelper::FormNotResize(this);
    this->InitForm();
    ui->txtUserPwd->setFocus();
}

FrmLogin::~FrmLogin()
{
    delete ui;
}
bool FrmLogin::eventFilter(QObject *obj, QEvent *event)
{
    //用户按下回车键,触发登录信号.
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
        if (keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter) {
            this->on_btnLogin_clicked();
            return true;
        }
    }
    return QObject::eventFilter(obj,event);
}

void FrmLogin::InitForm()
{
    {
    QSqlDatabase DbConn;
    DbConn=QSqlDatabase::addDatabase("QSQLITE");
    DbConn.setDatabaseName(myApp::AppPath+"table.db");

    //创建数据库连接并打开(如果数据库打开失败则终止应用程序)
    if (!DbConn.open()){
        myHelper::ShowMessageBoxError("打开数据库失败,程序将自动关闭！");
        this->close();
    }

    //加载用户信息链表到内存,当用户选择用户名和输入密码时,从内存中比较密码,速度极快.
    QString sql = "select [UserName],[UserPwd],[Level] from [UserInfo]";
    QSqlQuery *query = new QSqlQuery(DbConn);
    query->exec(sql);
    userList.clear();
    while(query->next())
    {
        //将对应用户的密码和类型存入链表,在用户登录时对比,比从数据库中查询速度要快.
        UserInfo temp;
        temp.userName = query->value(0).toString();
        temp.userPwd = query->value(1).toString();
        temp.level = query->value(2).toInt();

        ui->txtUserName->addItem(temp.userName);
        userList.append(temp);
    }
    delete query;
    }
    //DbConn.close();
    QString name;
    {
        name = QSqlDatabase::database().connectionName();
    }//超出作用域，隐含对象QSqlDatabase::database()被删除。
    QSqlDatabase::removeDatabase(name);
    //DbConn.removeDatabase("table.db");
    //对应显示软件标题和工程商信息.
    //ui->labTitle->setText(myApp::SoftTitle);
    ui->labCompany->setText(myApp::CompanyName);
    QPixmap pixmap(myApp::AppPath+myApp::LogoImage);
    ui->label_2->setPixmap(pixmap);
    //ui->txtUserName->setCurrentIndex(ui->txtUserName->findText(myApp::LastLoginer));
    //frm=new frmMain();//实例化主界面对象.
    myApp::WriteConfig();//保存配置

    ui->txtUserName->setCurrentIndex(0);
}

void FrmLogin::on_btnLogin_clicked()
{
    QString UserPwd=ui->txtUserPwd->text();

    int index=ui->txtUserName->currentIndex();

    if (UserPwd == userList.at(index).userPwd)
    {
        frm=new frmMain();
        //frmMain *frm=new frmMain();
        frm->currentUser = userList.at(index);
        //根据当前用户 显示或取消显示部分菜单
        frm->ReInitStyle();
        frm->show();
        this->hide();
    }else
    {
        myHelper::ShowMessageBoxError("密码错误,请重新输入!");
        ui->txtUserPwd->setFocus();
    }

}

void FrmLogin::on_btnClose_clicked()
{
    qApp->quit();
}
