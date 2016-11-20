#include "frmsavesound.h"
#include "ui_frmsavesound.h"
#include "iconhelper.h"
#include "myhelper.h"
FrmSaveSound::FrmSaveSound(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmSaveSound)
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
}

FrmSaveSound::~FrmSaveSound()
{
    delete ui;
}
void FrmSaveSound::ClearText()
{
    ui->leName->setText("");
}
void FrmSaveSound::on_btnOk_clicked()
{
    done(QDialog::Accepted);
    if(ui->leName->text()!="")
    {
        emit sigoperate(ui->leName->text());
        this->close();
    }
    else
    {
        myHelper::ShowMessageBoxError("名称不能为空");
    }

}

void FrmSaveSound::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmSaveSound::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmSaveSound::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}
