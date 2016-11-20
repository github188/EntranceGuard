#include "frmeditzhaomingpara.h"
#include "ui_frmeditzhaomingpara.h"

frmEditZhaoMingPara::frmEditZhaoMingPara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditZhaoMingPara)
{
    ui->setupUi(this);
    model=0;
    InitStyle();
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(btnEdit()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

frmEditZhaoMingPara::~frmEditZhaoMingPara()
{
    delete ui;
}
void frmEditZhaoMingPara::InitStyle()
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
    for(int i=0;i<24;i++)
    {
        ui->cbStartHour->addItem(QString::number(i));
        ui->cbStopHour->addItem(QString::number(i));
    }
    for(int i=0;i<60;i++)
    {
        ui->cbStartMinute->addItem(QString::number(i));
        ui->cbStopMinute->addItem(QString::number(i));
    }
}
void frmEditZhaoMingPara::initPara(paraZhaoMing paraTemp, quint8 mod)
{
    model=mod;
    para = paraTemp;
    ui->cbModel->setCurrentIndex(para.Model);
    ui->cbStartHour->setCurrentIndex(para.startHour);
    ui->cbStartMinute->setCurrentIndex(para.startMinute);
    ui->cbStopHour->setCurrentIndex(para.endHour);
    ui->cbStopMinute->setCurrentIndex(para.endMinute);
    if(para.manEnable)
        ui->checkboxWhenUsed->setChecked(true);
    else
        ui->checkboxWhenUsed->setChecked(false);


}
bool frmEditZhaoMingPara::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmEditZhaoMingPara::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmEditZhaoMingPara::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmEditZhaoMingPara::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmEditZhaoMingPara::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmEditZhaoMingPara::on_btnMenu_Max_clicked()
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

void frmEditZhaoMingPara::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void frmEditZhaoMingPara::btnEdit()
{
    para.Model = ui->cbModel->currentIndex();
    para.startHour = ui->cbStartHour->currentIndex();
    para.startMinute = ui->cbStartMinute->currentIndex();
    para.endHour = ui->cbStopHour->currentIndex();
    para.endMinute = ui->cbStopMinute->currentIndex();
    switch (model)
    {
    case 0:
        emit sigWriteZhaoMingPara(&para);
        break;
    case 1:
        emit sigSaveZhaoMingPara(&para);
        break;
    case 2://批量修改区域参数
        emit sigSaveAreaZhaoMingPara(&para);
        break;
    default:
        break;
    }

    this->close();
}
void frmEditZhaoMingPara::btnCancel()
{
    this->close();
}

void frmEditZhaoMingPara::on_checkboxWhenUsed_clicked(bool checked)
{
    if(checked)
        para.manEnable = 1;
    else
        para.manEnable = 0;
}
