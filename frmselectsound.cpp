#include "frmselectsound.h"
#include "ui_frmselectsound.h"

FrmSelectSound::FrmSelectSound(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmSelectSound)
{
    ui->setupUi(this);
    tableModel=NULL;
    //设置只读属性
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    InitStyle();
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    connect(ui->btnTestPlaySound,SIGNAL(clicked(bool)),this,SLOT(btnPlaySound()));
}

FrmSelectSound::~FrmSelectSound()
{
    delete ui;
}
void FrmSelectSound::InitStyle()
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

bool FrmSelectSound::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmSelectSound::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmSelectSound::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}
void FrmSelectSound::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmSelectSound::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmSelectSound::on_btnMenu_Max_clicked()
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

void FrmSelectSound::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void FrmSelectSound::btnCancel()
{
    this->close();
}

void FrmSelectSound::setTreeViewModel(QSqlTableModel *model)
{

    tableModel = model;
    ui->tableView->setModel(tableModel);

    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);
    ui->tableView->setColumnWidth(0,317);
    ui->tableView->setCurrentIndex(tableModel->index(0,0));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void FrmSelectSound::on_btnOk_clicked()
{
    done(QDialog::Accepted);
    QString name = tableModel->data(ui->tableView->currentIndex()).toString();
    if(name!="")
    {
        emit sigoperate(name);
        this->close();
    }
    else
    {
        myHelper::ShowMessageBoxError("名称不能为空");
    }

}
void FrmSelectSound::btnPlaySound()
{
    QModelIndex index = ui->tableView->currentIndex();
    QString name = tableModel->data(index).toString();
    emit sigPlaySound(name);
}
