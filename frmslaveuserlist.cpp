#include "frmslaveuserlist.h"
#include "ui_frmslaveuserlist.h"
#include "../myhelper.h"
FrmSlaveUserList::FrmSlaveUserList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmSlaveUserList)
{
    ui->setupUi(this);
    InitStyle();
    connect(ui->btnClose,SIGNAL(clicked(bool)),this,SLOT(btnClose()));
    connect(ui->btnDelete,SIGNAL(clicked(bool)),this,SLOT(btnDelete()));
    connect(ui->btnReturnNormalStatus,SIGNAL(clicked(bool)),this,SLOT(btnReturnNormalStatus()));
}

FrmSlaveUserList::~FrmSlaveUserList()
{
    delete ui;
}
void FrmSlaveUserList::setModel(quint8 model)
{
    switch(model)
    {
    case 0://显示用户列表
        ui->btnDelete->setVisible(false);
        break;
    case 1://删除用户列表
        ui->btnDelete->setVisible(true);
        break;
    }
}
void FrmSlaveUserList::InitStyle()
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

    ui->btnDelete->setVisible(false);

    QStringList header;
    header<<"用户列表";

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//关键
}

bool FrmSlaveUserList::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmSlaveUserList::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmSlaveUserList::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmSlaveUserList::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmSlaveUserList::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmSlaveUserList::on_btnMenu_Max_clicked()
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

void FrmSlaveUserList::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void FrmSlaveUserList::DisplayKeyList(QList<QByteArray> keyList)
{
    ui->tableWidget->setRowCount(keyList.count());
    for(int i=0;i<keyList.count();i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(myHelper::ByteArrayToHexStr(keyList.at(i))));
    }
}
void FrmSlaveUserList::slotAddKey(QByteArray key)
{
    int rowNum = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowNum);
    ui->tableWidget->setItem(rowNum,0,new QTableWidgetItem(myHelper::ByteArrayToHexStr(key)));
}
void FrmSlaveUserList::btnReturnNormalStatus()
{
    emit sigReturnNormalStatus();
}
void FrmSlaveUserList::btnClose()
{
    emit sigReturnNormalStatus();
    this->close();
}
void FrmSlaveUserList::btnDelete()
{
    emit sigDeleteKeyId(ui->tableWidget->currentRow());
}
