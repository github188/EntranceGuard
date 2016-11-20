#include "frmeditparamater.h"
#include "ui_frmeditparamater.h"

frmEditParamater::frmEditParamater(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditParamater)
{
    ui->setupUi(this);
    model=0;//0-设备的参数 1－模板中的参数
    editFlag=false;
    InitStyle();
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(btnEdit()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

frmEditParamater::~frmEditParamater()
{
    delete ui;
}
void frmEditParamater::InitStyle()
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
void frmEditParamater::initPara(paraFangHuCang paraTemp, quint8 mod)
{
    model = mod;
    para = paraTemp;
    ui->cbLockModel->setCurrentIndex(para.lockModel);
    ui->cbOpenLockTime->setCurrentIndex(para.openLockTime-1);
    ui->cbManDetectDeviceModel->setCurrentIndex(para.peopleEquModel);
    ui->cbManDetectInputModel->setCurrentIndex(para.signalModel);
    ui->lefanStartTemperature->setText(QString::number(para.fanRunTemperature));
    ui->leOperatTime->setText(QString::number(para.OptBussinessTime));
    ui->leRemaindInterval->setText(QString::number(para.warnningDelayTime));
    ui->leRemaindTime->setText(QString::number(para.timeOutRemind));
    ui->leWaitTime->setText(QString::number(para.noManOpenLockTime));
    if(para.fangQiewarnning)
        ui->rbEnable->setChecked(true);
    else
        ui->rbDisable->setChecked(true);

    if(para.kongCangLockorNot)
        ui->rBYes->setChecked(true);
    else
        ui->rBNo->setChecked(true);

}

bool frmEditParamater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmEditParamater::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmEditParamater::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmEditParamater::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmEditParamater::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmEditParamater::on_btnMenu_Max_clicked()
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

void frmEditParamater::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void frmEditParamater::btnEdit()
{
    if(editFlag)
        return;
    para.lockModel = ui->cbLockModel->currentIndex();
    para.openLockTime = ui->cbOpenLockTime->currentIndex()+1;
    para.peopleEquModel = ui->cbManDetectDeviceModel->currentIndex();
    para.signalModel = ui->cbManDetectInputModel->currentIndex();
    para.fanRunTemperature = ui->lefanStartTemperature->text().toInt();
    para.OptBussinessTime = ui->leOperatTime->text().toInt();
    para.warnningDelayTime = ui->leRemaindInterval->text().toInt();
    para.timeOutRemind = ui->leRemaindTime->text().toInt();
    para.noManOpenLockTime = ui->leWaitTime->text().toInt();
    switch (model)
    {
    case 0:
        emit sigWriteFangHuCangPara(&para);
        break;
    case 1:
        emit sigSaveFangHuCangPara(&para);
        break;
    case 2:
        emit sigSaveAreaFangHuCangPara(&para);
        break;
    default:
        break;
    }

    this->close();
}
void frmEditParamater::btnCancel()
{
    this->close();
}

void frmEditParamater::on_rBYes_clicked(bool checked)
{
    if(checked)
        para.kongCangLockorNot=1;
    else
        para.kongCangLockorNot=0;

}

void frmEditParamater::on_rBNo_clicked(bool checked)
{
    if(checked)
        para.kongCangLockorNot=0;
    else
        para.kongCangLockorNot=1;
}

void frmEditParamater::on_rbEnable_clicked(bool checked)
{
    if(checked)
        para.fangQiewarnning=1;
    else
        para.fangQiewarnning=0;
}

void frmEditParamater::on_rbDisable_clicked(bool checked)
{
    if(checked)
        para.fangQiewarnning=0;
    else
        para.fangQiewarnning=1;
}

void frmEditParamater::on_lefanStartTemperature_editingFinished()
{
    int value=ui->lefanStartTemperature->text().toInt();
    if((value<0)||(value>40))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("数据超出限制范围！", 2);
        msg->exec();
        ui->lefanStartTemperature->setText("0");
        editFlag=true;
        return;
    }
    else
    {

    }
    editFlag=false;
}

void frmEditParamater::on_leWaitTime_editingFinished()
{
    int value=ui->leWaitTime->text().toInt();
    if((value<0)||(value>600))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("数据超出限制范围！", 2);
        msg->exec();
        ui->leWaitTime->setText("0");
        editFlag=true;
        return;
    }
    editFlag=false;
}

void frmEditParamater::on_leOperatTime_editingFinished()
{
    int value=ui->leOperatTime->text().toInt();
    if((value<0)||(value>30))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("数据超出限制范围！", 2);
        msg->exec();
        ui->leOperatTime->setText("0");
        editFlag=true;
        return;
    }
    editFlag=false;
}

void frmEditParamater::on_leRemaindTime_editingFinished()
{
    int value=ui->leRemaindTime->text().toInt();
    if((value<0)||(value>120))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("数据超出限制范围！", 2);
        msg->exec();
        ui->leRemaindTime->setText("0");
        editFlag=true;
        return;
    }
    editFlag=false;
}

void frmEditParamater::on_leRemaindInterval_editingFinished()
{
    int value=ui->leRemaindInterval->text().toInt();
    if((value<0)||(value>120))
    {
        frmMessageBox *msg = new frmMessageBox();
        msg->SetMessage("数据超出限制范围！", 2);
        msg->exec();
        ui->leRemaindInterval->setText("0");
        editFlag=true;
        return;
    }
    editFlag=false;
}
