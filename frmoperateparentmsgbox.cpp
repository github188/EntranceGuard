#include "frmoperateparentmsgbox.h"
#include "ui_frmoperateparentmsgbox.h"
#include "frmmessagebox.h"
#include "ui_frmmessagebox.h"
#include "iconhelper.h"
#include "myhelper.h"

FrmOperateParentMsgBox::FrmOperateParentMsgBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmOperateParentMsgBox)
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

FrmOperateParentMsgBox::~FrmOperateParentMsgBox()
{
    delete ui;
}
void FrmOperateParentMsgBox::SetMessage(const QString &msg, int type)
{
    QString name=msg;
    if (type == 0)
    {
        ui->leName->setText(name);
        ui->lab_Title->setText("新建区域");
    } else if (type == 1)
    {
        ui->leName->setText(name);
        ui->lab_Title->setText("修改区域");
    } else if (type == 2)
    {
        ui->lab_Title->setText("错误");
    }
}

void FrmOperateParentMsgBox::on_btnOk_clicked()
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

void FrmOperateParentMsgBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmOperateParentMsgBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmOperateParentMsgBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

