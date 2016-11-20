#include "frmaddslave.h"
#include "ui_frmaddslave.h"

frmAddSlave::frmAddSlave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmAddSlave)
{
    ui->setupUi(this);
    searchedPara = NULL;
    InitStyle();
    connect(ui->btnConnect,SIGNAL(clicked(bool)),this,SLOT(btnConnect()));
    connect(ui->btnRegister,SIGNAL(clicked(bool)),this,SLOT(RegisterSlave()));
    connect(ui->btnClose,SIGNAL(clicked(bool)),this,SLOT(btnClose()));
}

frmAddSlave::~frmAddSlave()
{
    delete ui;
}
void frmAddSlave::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    location = this->geometry();
    max = false;
    mousePressed = false;
    //设置窗体关闭时自动释放内存
    //this->setAttribute(Qt::WA_DeleteOnClose);
    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    //IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

}

bool frmAddSlave::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmAddSlave::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmAddSlave::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmAddSlave::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmAddSlave::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmAddSlave::on_btnMenu_Max_clicked()
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

void frmAddSlave::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void frmAddSlave::btnConnect()
{
    emit sigConnectToSlave(ui->leIp->text());
}

void frmAddSlave::DisplayResult(bool registered,paraData *searched)
{
    bregistered = registered;
    searchedPara = searched;
    if(bregistered)
    {
        ui->lbRegistered->setText("已注册");
    }
    else
    {
        ui->lbRegistered->setText("未注册");
    }
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->horizontalHeader()->hide();//隐藏表头
    ui->tableWidget->verticalHeader()->hide();//隐藏行号
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,267);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("名称"));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("设备模式"));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("版本号"));
    ui->tableWidget->setItem(3,0,new QTableWidgetItem("MAC地址"));
    ui->tableWidget->setItem(4,0,new QTableWidgetItem("IP地址"));
    ui->tableWidget->setItem(5,0,new QTableWidgetItem("联网模式"));

    ui->tableWidget->setItem(0,1,new QTableWidgetItem(searchedPara->name));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(searchedPara->getSlaveType()));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(searchedPara->version));
    ui->tableWidget->setItem(3,1,new QTableWidgetItem(searchedPara->net.macAddr));
    ui->tableWidget->setItem(4,1,new QTableWidgetItem(searchedPara->net.ip));
    ui->tableWidget->setItem(5,1,new QTableWidgetItem(searchedPara->net.networkModel?"客户端模式":"服务器模式"));
}
void frmAddSlave::RegisterSlave()
{   if(!bregistered && (searchedPara != NULL))
    {
        emit sigRegisterSlave(searchedPara);
    }
    //this->close();
}
void frmAddSlave::btnClose()
{
    this->close();
}
