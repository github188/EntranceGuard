#include "frmusemodule.h"
#include "ui_frmusemodule.h"
#include "iconhelper.h"
#include "myhelper.h"
FrmUseModule::FrmUseModule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmUseModule)
{
    ui->setupUi(this);
    InitStyle();
    connect(ui->btnOK,SIGNAL(clicked(bool)),this,SLOT(btnOK()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    QStringList header;
    header<<"模版名称"<<"适用版本号";
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnWidth(0,317);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//关键
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

FrmUseModule::~FrmUseModule()
{
    delete ui;
}
void FrmUseModule::InitStyle()
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

bool FrmUseModule::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmUseModule::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmUseModule::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmUseModule::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmUseModule::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmUseModule::on_btnMenu_Max_clicked()
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

void FrmUseModule::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void FrmUseModule::btnOK()
{
    this->close();
    emit sigSelectModule(ui->tableWidget->currentIndex());
}
void FrmUseModule::btnCancel()
{
    this->close();
}
void FrmUseModule::slotSetTableModel(QList<SlaveVersion *> versionList)
{
    slaveVersionList= versionList;
    ui->tableWidget->setRowCount(slaveVersionList.length());
    for(int i=0;i<slaveVersionList.length();i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(slaveVersionList.at(i)->name));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(slaveVersionList.at(i)->version));
    }
}
