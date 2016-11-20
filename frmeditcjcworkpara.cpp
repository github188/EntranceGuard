#include "frmeditcjcworkpara.h"
#include "ui_frmeditcjcworkpara.h"

FrmEditCJCWorkPara::FrmEditCJCWorkPara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEditCJCWorkPara)
{
    ui->setupUi(this);
    model=0;//0-设备的参数 1－模板中的参数
    InitStyle();
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(btnEdit()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

FrmEditCJCWorkPara::~FrmEditCJCWorkPara()
{
    delete ui;
}
void FrmEditCJCWorkPara::InitStyle()
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

    ui->cbSetGuardDelayTime->clear();
    for(int i=0;i<91;i++)
    {
        ui->cbSetGuardDelayTime->addItem(QString::number(i));
    }
}

void FrmEditCJCWorkPara::initPara(paraFangHuCang paraTemp, quint8 mod)
{

    model = mod;
    para = paraTemp;

    ui->cbOpenLockTime->setCurrentIndex(para.openLockTime);
    ui->cbSetGuardDelayTime->setCurrentIndex(para.setGuardDelayTime);
    ui->cbInModel->setCurrentIndex(para.inDoorModel);
    ui->cbOutModel->setCurrentIndex(para.outDoorModel);
    ui->cbUseNum->setCurrentIndex(para.userNum);
    //系统报警状态
    if(para.systemAlarmStatus)
        ui->rbSetGuard->setChecked(true);
    else
        ui->rbResetGuard->setChecked(true);

    //值守状态
    if(para.isMonitorOrNot)
        ui->rbExistMan->setChecked(true);
    else
        ui->rbNoMan->setChecked(true);

}
/*
void FrmEditCJCWorkPara::initPara(paraAlarm *paraTemp, quint8 mod)
{

    model = mod;
    para = *paraTemp;
    //按钮报警
    if(para.btnAlarmEnable)
        ui->rbBtnEnable->setChecked(true);
    else
        ui->rbBtnDisable->setChecked(true);
    //防切割报警
    if(para.cutAlarmEnable)
        ui->rbFangQieEnable->setChecked(true);
    else
        ui->rbFangQieDisable->setChecked(true);
    //震动报警
    if(para.zhengDongAlarmEnable)
        ui->rbZhenDongEnable->setChecked(true);
    else
        ui->rbZhenDongDisable->setChecked(true);
    //烟雾报警
    if(para.yanWuAlarmEnable)
        ui->rbYanWuEnable->setChecked(true);
    else
        ui->rbYanWuDisable->setChecked(true);
    //玻璃破碎报警
    if(para.boLiAlarmEnable)
        ui->rbBoLiEnable->setChecked(true);
    else
        ui->rbBoLiDisable->setChecked(true);
    //水浸报警
    if(para.shuiQinAlarmEnable)
        ui->rbShuiQinEnable->setChecked(true);
    else
        ui->rbShuiQinDisable->setChecked(true);
    //温度报警
    if(para.tempAlarmEnable)
        ui->rbTempEnable->setChecked(true);
    else
        ui->rbTempDisable->setChecked(true);


}
*/
bool FrmEditCJCWorkPara::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmEditCJCWorkPara::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmEditCJCWorkPara::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmEditCJCWorkPara::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmEditCJCWorkPara::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmEditCJCWorkPara::on_btnMenu_Max_clicked()
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

void FrmEditCJCWorkPara::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void FrmEditCJCWorkPara::btnEdit()
{
    para.openLockTime = ui->cbOpenLockTime->currentIndex();
    para.setGuardDelayTime = ui->cbSetGuardDelayTime->currentIndex();
    para.inDoorModel = ui->cbInModel->currentIndex();
    para.outDoorModel = ui->cbOutModel->currentIndex();
    para.userNum = ui->cbUseNum->currentIndex();

    emit sigWriteCjcWorkPara(&para);
    this->close();
}
void FrmEditCJCWorkPara::btnCancel()
{
    this->close();
}


void FrmEditCJCWorkPara::on_rbSetGuard_clicked()
{
    para.systemAlarmStatus=1;
}

void FrmEditCJCWorkPara::on_rbResetGuard_clicked()
{
    para.systemAlarmStatus=0;
}

void FrmEditCJCWorkPara::on_rbExistMan_clicked()
{
    para.isMonitorOrNot=1;
}

void FrmEditCJCWorkPara::on_rbNoMan_clicked()
{
    para.isMonitorOrNot=0;
}
