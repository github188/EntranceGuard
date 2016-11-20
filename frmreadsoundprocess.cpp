#include "frmreadsoundprocess.h"
#include "ui_frmreadsoundprocess.h"
#include "iconhelper.h"
#include "myhelper.h"
FrmReadSoundProcess::FrmReadSoundProcess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmReadSoundProcess)
{
    ui->setupUi(this);
    this->mousePressed = false;
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗体关闭时自动释放内存
    //this->setAttribute(Qt::WA_DeleteOnClose);
    //设置图形字体
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //关联关闭按钮
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    //窗体居中显示
    myHelper::FormInCenter(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(0);

    ui->pBarMain->setMinimum(0);
    ui->pBarMain->setMaximum(1);
    ui->pBarMain->setValue(0);
}

FrmReadSoundProcess::~FrmReadSoundProcess()
{
    delete ui;
}
void FrmReadSoundProcess::on_btnOk_clicked()
{

}

void FrmReadSoundProcess::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmReadSoundProcess::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmReadSoundProcess::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}
//设置主进程最大数值
void FrmReadSoundProcess::setPBarMainMaxValue(quint64 maxValue)
{
    ui->pBarMain->setMaximum(maxValue);
}
void FrmReadSoundProcess::setProcessMaxValue(quint64 maxValue)
{
    ui->progressBar->setMaximum(maxValue);
}
void FrmReadSoundProcess::setPBarMainCurrentValue(quint64 currentValue)
{
    ui->pBarMain->setValue(currentValue);
}
void FrmReadSoundProcess::setProcessCurrentValue(quint64 currentValue)
{
    ui->progressBar->setValue(currentValue);
}
void FrmReadSoundProcess::slotSetWindowsText(QString text)
{
    ui->lab_Title->setText(text);
}
