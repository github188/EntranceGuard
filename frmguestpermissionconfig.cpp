#include "frmguestpermissionconfig.h"
#include "ui_frmguestpermissionconfig.h"

frmGuestPermissionConfig::frmGuestPermissionConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmGuestPermissionConfig)
{
    ui->setupUi(this);
    InitStyle();
    connect(ui->btnOK,SIGNAL(clicked(bool)),this,SLOT(btnOK()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
}

frmGuestPermissionConfig::~frmGuestPermissionConfig()
{
    delete ui;
}
void frmGuestPermissionConfig::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

}
void frmGuestPermissionConfig::initPara(UserPermission guest)
{
    //系统权限
    user=guest;
    //修改系统参数 0-禁止 1-允许
    if(guest.permitEditSystemPar)
    {
        ui->cbPermitEditSystemPar->setChecked(true);
    }
    else
    {
        ui->cbPermitEditSystemPar->setChecked(false);
    }
    //修改区域信息 0-禁止 1-允许
    if(guest.permitEditAreaInfo)
    {
        ui->cbPermitEditAreaInfo->setChecked(true);
    }
    else
    {
        ui->cbPermitEditAreaInfo->setChecked(false);
    }
   //注册防护舱 0-禁止 1-允许
    if(guest.permitRegFangHuCang)
    {
        ui->cbPermitRegFangHuCang->setChecked(true);
    }
    else
    {
        ui->cbPermitRegFangHuCang->setChecked(false);
    }
   //注销防护舱 0-禁止 1-允许
    if(guest.permitLogoutFangHuCang)
    {
        ui->cbPermitLogoutFangHuCang->setChecked(true);
    }
    else
    {
        ui->cbPermitLogoutFangHuCang->setChecked(false);
    }
   //编辑语音库 0-禁止 1-允许
    if(guest.permitEditSoundLiabrary)
    {
        ui->cbPermitEditSoundLiabrary->setChecked(true);
    }
    else
    {
        ui->cbPermitEditSoundLiabrary->setChecked(false);
    }
//编辑参数模板库 0-禁止 1-允许
    if(guest.permitEditModule)
    {
        ui->cbPermitEditModule->setChecked(true);
    }
    else
    {
        ui->cbPermitEditModule->setChecked(false);
    }
    //设备权限
    //编辑防护舱工作参数 0-禁止 1-允许
    if(guest.permitEditFangHuCangPar)
    {
        ui->cbPermitEditFangHuCangPar->setChecked(true);
    }
    else
    {
        ui->cbPermitEditFangHuCangPar->setChecked(false);
    }
    //编辑防护舱照明参数 0-禁止 1-允许
    if(guest.permitEditZhaoMingPar)
    {
        ui->cbPermitEditZhaoMingPar->setChecked(true);
    }
    else
    {
        ui->cbPermitEditZhaoMingPar->setChecked(false);
    }
//编辑防护舱语音设置 0-禁止 1-允许
    if(guest.permitEditVoicePar)
    {
        ui->cbPermitEditVoicePar->setChecked(true);
    }
    else
    {
        ui->cbPermitEditVoicePar->setChecked(false);
    }
//校准时间 0-禁止 1-允许
    if(guest.permitAdjustTime)
    {
        ui->cbPermitAdjustTime->setChecked(true);
    }
    else
    {
        ui->cbPermitAdjustTime->setChecked(false);
    }
//远程开门 0-禁止 1-允许
    if(guest.permitOpenDoor)
    {
        ui->cbPermitOpenDoor->setChecked(true);
    }
    else
    {
        ui->cbPermitOpenDoor->setChecked(false);
    }
//启用/停用 防护舱 0-禁止 1-允许
    if(guest.permitUseorNotusedFangHuCang)
    {
        ui->cbPermitUseorNotusedFangHuCang->setChecked(true);
    }
    else
    {
        ui->cbPermitUseorNotusedFangHuCang->setChecked(false);
    }
//闭锁/解锁 防护舱 0-禁止 1-允许
    if(guest.permitLockorOpenFangHuCang)
    {
        ui->cbPermitLockorOpenFangHuCang->setChecked(true);
    }
    else
    {
        ui->cbPermitLockorOpenFangHuCang->setChecked(false);
    }
//编辑LED节目内容
    if(guest.permitEditLedText)
    {
        ui->cbPermitEditLedText->setChecked(true);
    }
    else
    {
        ui->cbPermitEditLedText->setChecked(false);
    }

    //编辑LED节目内容
    if(guest.permitChangeAdmin)
    {
        ui->cbPermitChangeAdmin->setChecked(true);
    }
    else
    {
        ui->cbPermitChangeAdmin->setChecked(false);
    }
}
bool frmGuestPermissionConfig::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmGuestPermissionConfig::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmGuestPermissionConfig::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmGuestPermissionConfig::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmGuestPermissionConfig::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmGuestPermissionConfig::on_btnMenu_Max_clicked()
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

void frmGuestPermissionConfig::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void frmGuestPermissionConfig::btnOK()
{
    //系统权限

    //修改系统参数 0-禁止 1-允许
    if(ui->cbPermitEditSystemPar->checkState()==Qt::Checked)
    {
        user.permitEditSystemPar=true;
    }
    else
    {
        user.permitEditSystemPar=false;
    }
    //修改区域信息 0-禁止 1-允许
    if(ui->cbPermitEditAreaInfo->checkState()==Qt::Checked)
    {
        user.permitEditAreaInfo = true;
    }
    else
    {
        user.permitEditAreaInfo = false;
    }
   //注册防护舱 0-禁止 1-允许
    if(ui->cbPermitRegFangHuCang->checkState()==Qt::Checked)
    {
        user.permitRegFangHuCang = true;
    }
    else
    {
        user.permitRegFangHuCang = false;
    }
   //注销防护舱 0-禁止 1-允许
    if(ui->cbPermitLogoutFangHuCang->checkState()==Qt::Checked)
    {
        user.permitLogoutFangHuCang=true;
    }
    else
    {
        user.permitLogoutFangHuCang=false;
    }
   //编辑语音库 0-禁止 1-允许
    if(ui->cbPermitEditSoundLiabrary->checkState()==Qt::Checked)
    {
        user.permitEditSoundLiabrary=true;
    }
    else
    {
        user.permitEditSoundLiabrary=false;
    }
//编辑参数模板库 0-禁止 1-允许
    if(ui->cbPermitEditModule->checkState()==Qt::Checked)
    {
        user.permitEditModule=true;
    }
    else
    {
        user.permitEditModule=false;
    }
    //设备权限
    //编辑防护舱工作参数 0-禁止 1-允许
    if(ui->cbPermitEditFangHuCangPar->checkState()==Qt::Checked)
    {
        user.permitEditFangHuCangPar=true;
    }
    else
    {
        user.permitEditFangHuCangPar=false;
    }
    //编辑防护舱照明参数 0-禁止 1-允许
    if(ui->cbPermitEditZhaoMingPar->checkState()==Qt::Checked)
    {
        user.permitEditZhaoMingPar=true;
    }
    else
    {
        user.permitEditZhaoMingPar=false;
    }
//编辑防护舱语音设置 0-禁止 1-允许
    if(ui->cbPermitEditVoicePar->checkState()==Qt::Checked)
    {
        user.permitEditVoicePar=true;
    }
    else
    {
        user.permitEditVoicePar=false;
    }
//校准时间 0-禁止 1-允许
    if(ui->cbPermitAdjustTime->checkState()==Qt::Checked)
    {
        user.permitAdjustTime=true;
    }
    else
    {
        user.permitAdjustTime=false;
    }
//远程开门 0-禁止 1-允许
    if(ui->cbPermitOpenDoor->checkState()==Qt::Checked)
    {
        user.permitOpenDoor=true;
    }
    else
    {
        user.permitOpenDoor=false;
    }
//启用/停用 防护舱 0-禁止 1-允许
    if(ui->cbPermitUseorNotusedFangHuCang->checkState()==Qt::Checked)
    {
        user.permitUseorNotusedFangHuCang = true;
    }
    else
    {
        user.permitUseorNotusedFangHuCang = false;
    }
//闭锁/解锁 防护舱 0-禁止 1-允许
    if(ui->cbPermitLockorOpenFangHuCang->checkState()==Qt::Checked)
    {
        user.permitLockorOpenFangHuCang=true;
    }
    else
    {
        user.permitLockorOpenFangHuCang=false;
    }
//编辑LED节目内容
    if(ui->cbPermitEditLedText->checkState()==Qt::Checked)
    {
        user.permitEditLedText=true;
    }
    else
    {
        user.permitEditLedText=false;
    }
    //更换管理员
    if(ui->cbPermitChangeAdmin->checkState()==Qt::Checked)
    {
        user.permitChangeAdmin=true;
    }
    else
    {
        user.permitChangeAdmin=false;
    }
    emit sigWriteUserPermitPara(&user);
    this->close();
}
void frmGuestPermissionConfig::btnCancel()
{
    this->close();
}

