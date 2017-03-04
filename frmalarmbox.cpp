#include "frmalarmbox.h"
#include "ui_frmalarmbox.h"
extern QString sFilePath;
frmAlarmBox::frmAlarmBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmAlarmBox)
{
    ui->setupUi(this);
    InitStyle();
    for(int i=0;i<10;i++)
    {
        enable[i]=0;
    }
    mMediaObj = Phonon::createPlayer(Phonon::MusicCategory);
    connect(mMediaObj, SIGNAL(finished()), SLOT(finished()));
    connect(ui->btnOK,SIGNAL(clicked(bool)),this,SLOT(btnOK()));
}

frmAlarmBox::~frmAlarmBox()
{
    delete ui;
}
void frmAlarmBox::InitStyle()
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

bool frmAlarmBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmAlarmBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmAlarmBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmAlarmBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmAlarmBox::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmAlarmBox::on_btnMenu_Max_clicked()
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

void frmAlarmBox::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void frmAlarmBox::btnOK()
{
    mMediaObj->stop();
    emit sigClearAlarm();
    this->close();
}
void frmAlarmBox::setSlave(Equipment * slave)
{
    ui->leName->setText(slave->GetEquName());
    ui->leVersion->setText(slave->GetEquVersion());
    ui->leMac->setText(slave->GetEquMac());
    paraData * data = slave->GetEquAll();
    equStatus sta = data->slaveStatus;
    quint8 wave=0;
    if(sta.boLiAlarmStatus)
    {
        ui->txtAlarmText->append("玻璃破碎报警:出现！");
        enable[0]=1;
        wave=1;
    }
    if(sta.buttonAlarmStatus)
    {
        ui->txtAlarmText->append("按钮报警:出现！");
        enable[1]=1;
        wave=1;
    }
    if(sta.cutAlarmStatus)
    {
        ui->txtAlarmText->append("切割报警:出现！");
        enable[2]=1;
        wave=1;
    }
    if(sta.shuiQinAlarmStatus)
    {
        ui->txtAlarmText->append("水浸报警:出现！");
        enable[3]=1;
        wave=1;
    }
    if(sta.tempAlarmStatus)
    {
        ui->txtAlarmText->append("温度报警:出现！");
        enable[4]=1;
        wave=1;
    }
    if(sta.yanWuAlarmStatus)
    {
        ui->txtAlarmText->append("烟雾报警:出现！");
        enable[5]=1;
        wave=1;
    }
    if(sta.zhengDongAlarmStatus)
    {
        ui->txtAlarmText->append("震动报警:出现！");
        enable[6]=1;
        wave=1;
    }
    if(sta.doorCiAlarmStatus)
    {
        ui->txtAlarmText->append("非法开门报警:出现！");
        enable[7]=1;
        wave=1;
    }
    if(sta.existManAlarmStatus)
    {
        ui->txtAlarmText->append("非法入侵报警:出现！");
        enable[8]=1;
        wave=1;
    }
    //判断报警消失
    if(enable[0])
    {
        if(!sta.boLiAlarmStatus)
        {
            ui->txtAlarmText->append("非法入侵报警:消失！");
            enable[0]=0;
        }
    }
    if(enable[1])
    {
        if(!sta.buttonAlarmStatus)
        {
            ui->txtAlarmText->append("按钮报警:消失！");
            enable[1]=0;
        }
    }
    if(enable[2])
    {
        if(!sta.cutAlarmStatus)
        {
            ui->txtAlarmText->append("切割报警:消失！");
            enable[2]=0;
        }
    }
    if(enable[3])
    {
        if(!sta.shuiQinAlarmStatus)
        {
            ui->txtAlarmText->append("水浸报警:消失！");
            enable[3]=0;
        }
    }
    if(enable[4])
    {
        if(!sta.tempAlarmStatus)
        {
            ui->txtAlarmText->append("温度报警:消失！");
            enable[4]=0;
        }
    }
    if(enable[5])
    {
        if(!sta.yanWuAlarmStatus)
        {
            ui->txtAlarmText->append("烟雾报警:消失！");
            enable[5]=0;
        }
    }
    if(enable[6])
    {
        if(!sta.zhengDongAlarmStatus)
        {
            ui->txtAlarmText->append("震动报警:消失！");
            enable[6]=0;
        }
    }
    if(enable[7])
    {
        if(!sta.doorCiAlarmStatus)
        {
            ui->txtAlarmText->append("非法开门报警:消失！");
            enable[7]=0;
        }
    }

    if(enable[8])
    {
        if(!sta.existManAlarmStatus)
        {
            ui->txtAlarmText->append("非法入侵报警:消失！");
            enable[8]=0;
        }
    }
    //mMediaObj->clear();
    if(wave)
    {
        mMediaObj->setCurrentSource(sFilePath+"报警时软件播放声音.wav");
        mMediaObj->play();
    }
}
void frmAlarmBox::finished()
{
    mMediaObj->setCurrentSource(sFilePath+"报警时软件播放声音.wav");
    mMediaObj->play();
}
