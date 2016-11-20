#include "frmreadslavemsg.h"
#include "ui_frmreadslavemsg.h"
#include "iconhelper.h"
#include "myhelper.h"
frmReadSlaveMsg::frmReadSlaveMsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmReadSlaveMsg)
{
    ui->setupUi(this);
    this->mousePressed = false;
    ui->progressBar->setMinimum(0);
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    //设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
    //设置图形字体
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //关联关闭按钮
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    //窗体居中显示
    myHelper::FormInCenter(this);
}

frmReadSlaveMsg::~frmReadSlaveMsg()
{
    delete ui;
}
void frmReadSlaveMsg::on_btnOk_clicked()
{
    done(1);
    this->close();
}

void frmReadSlaveMsg::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmReadSlaveMsg::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}
void frmReadSlaveMsg::setProcess(quint32 currentValue,quint32 value)
{
    ui->progressBar->setMinimum(value);
    ui->progressBar->setValue(currentValue);
}
void frmReadSlaveMsg::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}
