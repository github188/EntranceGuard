#include "frmeditalarmpara.h"
#include "ui_frmeditalarmpara.h"

frmEditAlarmPara::frmEditAlarmPara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditAlarmPara)
{
    ui->setupUi(this);
    model=0;//0-设备的参数 1－模板中的参数
    InitStyle();
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(btnEdit()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

frmEditAlarmPara::~frmEditAlarmPara()
{
    delete ui;
}
void frmEditAlarmPara::InitStyle()
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
void frmEditAlarmPara::initPara(paraAlarm paraTemp, quint8 mod)
{

    model = mod;
    para = paraTemp;
    //按钮报警
    if(para.btnAlarmEnable)
        ui->rbBtnEnable->setChecked(true);
    else
        ui->rbBtnDisable->setChecked(true);
    //防切割报警
    if(para.cutAlarmEnable)
        ui->rbFangQieEnable->setChecked(true);
    else
        ui->rbFangQieDisable->setChecked(true);
    //震动报警
    if(para.zhengDongAlarmEnable)
        ui->rbZhenDongEnable->setChecked(true);
    else
        ui->rbZhenDongDisable->setChecked(true);
    //烟雾报警
    if(para.yanWuAlarmEnable)
        ui->rbYanWuEnable->setChecked(true);
    else
        ui->rbYanWuDisable->setChecked(true);
    //玻璃破碎报警
    if(para.boLiAlarmEnable)
        ui->rbBoLiEnable->setChecked(true);
    else
        ui->rbBoLiDisable->setChecked(true);
    //水浸报警
    if(para.shuiQinAlarmEnable)
        ui->rbShuiQinEnable->setChecked(true);
    else
        ui->rbShuiQinDisable->setChecked(true);
    //温度报警
    if(para.tempAlarmEnable)
        ui->rbTempEnable->setChecked(true);
    else
        ui->rbTempDisable->setChecked(true);
    //是否启用非法开门报警
    if(para.doorCiAlarmEnable)
        ui->rbIllegalOpenDoorEnable->setChecked(true);
    else
        ui->rbIllegalOpenDoorDisable->setChecked(true);
    //是否启用非法入侵报警
    if(para.existManAlarmEnable)
        ui->rbIllegalInEnable->setChecked(true);
    else
        ui->rbIllegalInDisable->setChecked(true);


}
void frmEditAlarmPara::initPara(paraAlarm *paraTemp, quint8 mod)
{

    model = mod;
    para = *paraTemp;
    //按钮报警
    if(para.btnAlarmEnable)
        ui->rbBtnEnable->setChecked(true);
    else
        ui->rbBtnDisable->setChecked(true);
    //防切割报警
    if(para.cutAlarmEnable)
        ui->rbFangQieEnable->setChecked(true);
    else
        ui->rbFangQieDisable->setChecked(true);
    //震动报警
    if(para.zhengDongAlarmEnable)
        ui->rbZhenDongEnable->setChecked(true);
    else
        ui->rbZhenDongDisable->setChecked(true);
    //烟雾报警
    if(para.yanWuAlarmEnable)
        ui->rbYanWuEnable->setChecked(true);
    else
        ui->rbYanWuDisable->setChecked(true);
    //玻璃破碎报警
    if(para.boLiAlarmEnable)
        ui->rbBoLiEnable->setChecked(true);
    else
        ui->rbBoLiDisable->setChecked(true);
    //水浸报警
    if(para.shuiQinAlarmEnable)
        ui->rbShuiQinEnable->setChecked(true);
    else
        ui->rbShuiQinDisable->setChecked(true);
    //温度报警
    if(para.tempAlarmEnable)
        ui->rbTempEnable->setChecked(true);
    else
        ui->rbTempDisable->setChecked(true);


}
bool frmEditAlarmPara::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmEditAlarmPara::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmEditAlarmPara::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmEditAlarmPara::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmEditAlarmPara::on_btnMenu_Close_clicked()
{
    this->close();
}

void frmEditAlarmPara::on_btnMenu_Max_clicked()
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

void frmEditAlarmPara::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void frmEditAlarmPara::btnEdit()
{
    emit sigWriteAlarmPara(&para);
    this->close();
}
void frmEditAlarmPara::btnCancel()
{
    this->close();
}

void frmEditAlarmPara::on_rbBtnEnable_clicked()
{

    para.btnAlarmEnable=1;

}

void frmEditAlarmPara::on_rbBtnDisable_clicked()
{

    para.btnAlarmEnable=0;

}

void frmEditAlarmPara::on_rbFangQieEnable_clicked()
{
    para.cutAlarmEnable=1;

}

void frmEditAlarmPara::on_rbFangQieDisable_clicked()
{

    para.cutAlarmEnable=0;

}

void frmEditAlarmPara::on_rbZhenDongEnable_clicked()
{

    para.zhengDongAlarmEnable=1;

}

void frmEditAlarmPara::on_rbZhenDongDisable_clicked()
{

    para.zhengDongAlarmEnable=0;

}

void frmEditAlarmPara::on_rbYanWuEnable_clicked()
{
    para.yanWuAlarmEnable=1;

}

void frmEditAlarmPara::on_rbYanWuDisable_clicked()
{
    para.yanWuAlarmEnable=0;

}

void frmEditAlarmPara::on_rbBoLiDisable_clicked()
{
    para.boLiAlarmEnable=0;

}

void frmEditAlarmPara::on_rbBoLiEnable_clicked()
{
    para.boLiAlarmEnable=1;

}

void frmEditAlarmPara::on_rbShuiQinDisable_clicked()
{
    para.shuiQinAlarmEnable=0;

}

void frmEditAlarmPara::on_rbShuiQinEnable_clicked()
{

    para.shuiQinAlarmEnable=1;

}

void frmEditAlarmPara::on_rbTempDisable_clicked()
{
    para.tempAlarmEnable=0;
}

void frmEditAlarmPara::on_rbTempEnable_clicked()
{
    para.tempAlarmEnable=1;
}

void frmEditAlarmPara::on_rbIllegalOpenDoorEnable_clicked()
{
    para.doorCiAlarmEnable=1;
}

void frmEditAlarmPara::on_rbIllegalOpenDoorDisable_clicked()
{
    para.doorCiAlarmEnable=0;
}

void frmEditAlarmPara::on_rbIllegalInEnable_clicked()
{
    para.existManAlarmEnable=1;
}

void frmEditAlarmPara::on_rbIllegalInDisable_clicked()
{
    para.existManAlarmEnable=0;
}
