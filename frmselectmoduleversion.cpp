#include "frmselectmoduleversion.h"
#include "ui_frmselectmoduleversion.h"

FrmSelectModuleVersion::FrmSelectModuleVersion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmSelectModuleVersion)
{
    ui->setupUi(this);
    InitStyle();
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    //connect(ui->btnTestPlaySound,SIGNAL(clicked(bool)),this,SLOT(btnPlaySound()));
}

FrmSelectModuleVersion::~FrmSelectModuleVersion()
{
    delete ui;
}
void FrmSelectModuleVersion::InitStyle()
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
//设置可选择的版本号
void FrmSelectModuleVersion::slotSetVersionInfoList(QList<VersionInfo*> list)
{
    for(int i=0;i<list.length();i++)
    {
        ui->comboBox->addItem(list.at(i)->name);
    }
}
void FrmSelectModuleVersion::setVoiceModel(QSqlTableModel *model)
{
    voiceModel = model;
}
bool FrmSelectModuleVersion::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmSelectModuleVersion::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmSelectModuleVersion::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}
void FrmSelectModuleVersion::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmSelectModuleVersion::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmSelectModuleVersion::on_btnMenu_Max_clicked()
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

void FrmSelectModuleVersion::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void FrmSelectModuleVersion::btnCancel()
{
    this->close();
}
//保存模板参数
void FrmSelectModuleVersion::slotSaveModule(QString name,paraData* para)
{
    emit sigSaveModule(name,para);
}
void FrmSelectModuleVersion::on_btnOk_clicked()
{
    /*
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
    */
    quint8 index = ui->comboBox->currentIndex();
    switch (index)
    {
    case 0:

        break;
    case 1:
    {
        paraData *para = new paraData();
        FrmModulePara *frm= new FrmModulePara();
        connect(frm,SIGNAL(sigSaveModulePara(QString,paraData*)),this,SLOT(slotSaveModule(QString,paraData*)));

        frm->setVoiceModel(voiceModel);
        frm->SetPara("新建模板",para,0);
        frm->SetTitleText("新建模板参数");
        this->hide();
        frm->exec();
        delete para;
        //关闭版本选择窗口
        this->close();
    }
        break;
    default:
        break;
    }
}

