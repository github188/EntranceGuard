#include "frmaddsound.h"
#include "ui_frmaddsound.h"

FrmAddSound::FrmAddSound(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAddSound)
{
    ui->setupUi(this);
    InitStyle();
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    connect(ui->btnClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnFile,SIGNAL(clicked(bool)),this,SLOT(btnFile()));
    connect(ui->btnOK,SIGNAL(clicked(bool)),this,SLOT(btnOK()));
}

FrmAddSound::~FrmAddSound()
{
    delete ui;
}
void FrmAddSound::InitStyle()
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

bool FrmAddSound::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmAddSound::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmAddSound::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmAddSound::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmAddSound::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmAddSound::on_btnMenu_Max_clicked()
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

void FrmAddSound::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void FrmAddSound::btnOK()
{
    fileName=ui->leName->text();
    if(fileName!="")
    {

        emit sigAddVoice(filePath,fileName);
    }
    this->close();
}
void FrmAddSound::btnFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("选择文件"), ".", tr("mp3 Files(*.mp3)"));

    if(path.length())
    {
        ui->tbFilePath->setText(path);
        filePath = path;
    } else
    {

    }
}
