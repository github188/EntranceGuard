#include "frmsoftwareinfo.h"
#include "ui_frmsoftwareinfo.h"
#include "./AppConfig/myapp.h"
frmSoftwareInfo::frmSoftwareInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSoftwareInfo)
{
    ui->setupUi(this);
    InitStyle();
}

frmSoftwareInfo::~frmSoftwareInfo()
{
    delete ui;
}
void frmSoftwareInfo::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    QPixmap pixmap(myApp::AppPath+myApp::LogoImage);
    ui->lb_ICO->setPixmap(pixmap);
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    ui->textBrowser->append("<p style='margin:5px'>名称："+myApp::SoftTitle);
    ui->textBrowser->append("版本号："+myApp::SoftVersion);
    ui->textBrowser->append("网址："+myApp::WebStation);
    ui->textBrowser->append("电话："+myApp::telephone);
    ui->textBrowser->append("手机："+myApp::cellphone+"<p>");
    ui->label_2->setText(myApp::CompanyName);
}

bool frmSoftwareInfo::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmSoftwareInfo::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmSoftwareInfo::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmSoftwareInfo::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmSoftwareInfo::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmSoftwareInfo::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void frmSoftwareInfo::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
