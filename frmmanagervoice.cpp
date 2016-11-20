#include "frmmanagervoice.h"
#include "ui_frmmanagervoice.h"

FrmManagerVoice::FrmManagerVoice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmManagerVoice)
{
    ui->setupUi(this);

    InitStyle();

    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(btnDelete()));
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this,SLOT(btnAdd()));
    connect(ui->btnTestPlaySound,SIGNAL(clicked(bool)),this,SLOT(btnPlaySound()));

}

FrmManagerVoice::~FrmManagerVoice()
{
    delete ui;
}
void FrmManagerVoice::InitStyle()
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

bool FrmManagerVoice::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmManagerVoice::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmManagerVoice::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmManagerVoice::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmManagerVoice::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmManagerVoice::on_btnMenu_Max_clicked()
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

void FrmManagerVoice::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void FrmManagerVoice::btnCancel()
{
    this->close();
}
void FrmManagerVoice::btnDelete()
{
    QModelIndex index = ui->tableView->currentIndex();
    QString name = tableModel->data(index).toString();
    emit sigDeleteVoice(name);
}
void FrmManagerVoice::setTreeViewModel(QSqlTableModel *model)
{
    tableModel = model;
    ui->tableView->setModel(tableModel);

    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);
    ui->tableView->setColumnWidth(0,317);
}
void FrmManagerVoice::btnAdd()
{
    FrmAddSound *frmAddSound = new FrmAddSound();
    connect(frmAddSound,SIGNAL(sigAddVoice(QString,QString)),this,SLOT(slotAddVoice(QString,QString)));
    frmAddSound->exec();
    //emit sigAddVoice();
}
void FrmManagerVoice::btnPlaySound()
{
    QModelIndex index = ui->tableView->currentIndex();
    QString name = tableModel->data(index).toString();
    emit sigPlaySound(name);
}
void FrmManagerVoice::DisableorEnableEdit(bool enable)
{
    ui->btnAdd->setEnabled(enable);
    ui->btnDel->setEnabled(enable);
}
void FrmManagerVoice::slotAddVoice(QString filePath,QString name)
{
    emit sigAddVoicetoDatabase(filePath,name);
}
void FrmManagerVoice::slotShowAddVoiceEndWindow(int end)
{
    switch (end)
    {
    case 0:
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("读取数据库错误！", 2);
        msg->exec();
    }
        break;
    case 1:
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("音乐文件已经存在！", 0);
        msg->exec();
    }
        break;
    case 2:
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage("无法创建文件", 2);
        msg->exec();
    }
        break;
    default:
        break;
    }
}
