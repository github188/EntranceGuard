#include "frmmodulepara.h"
#include "ui_frmmodulepara.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "./DATA/voiceinfobank.h"
extern QList<VoiceInfoBank> voiceLibrary;
FrmModulePara::FrmModulePara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmModulePara)
{
    ui->setupUi(this);
    pdat = NULL;
    voiceModel=NULL;
    model=0;
    //frmEditSlavePara = new frmEditParamater();
    //frmEditZhaoMing = new frmEditZhaoMingPara();
    //frmeditalarmpara = new frmEditAlarmPara();
    //frmSelectSound = new FrmSelectSound();
    ui->btnLed0Ok->setVisible(false);
    ui->btnLed1Ok->setVisible(false);
    ui->btnLed2Ok->setVisible(false);
    ui->tableWidgetFangHuCang->setColumnCount(4);
    ui->tableWidgetFangHuCang->setRowCount(6);
    ui->tableWidgetFangHuCang->setColumnWidth(0,140);
    ui->tableWidgetFangHuCang->setColumnWidth(1,60);
    ui->tableWidgetFangHuCang->setColumnWidth(2,160);
    ui->tableWidgetFangHuCang->setColumnWidth(3,60);
    ui->tableWidgetFangHuCang->horizontalHeader()->hide();//隐藏表头
    ui->tableWidgetFangHuCang->verticalHeader()->hide();//隐藏行号
    ui->tableWidgetFangHuCang->setItem(0,0,new QTableWidgetItem("锁具类型"));
    ui->tableWidgetFangHuCang->setItem(1,0,new QTableWidgetItem("开锁信号持续时间"));
    ui->tableWidgetFangHuCang->setItem(2,0,new QTableWidgetItem("人体检测信号输入方式"));
    ui->tableWidgetFangHuCang->setItem(3,0,new QTableWidgetItem("人体检测装置类型"));
    ui->tableWidgetFangHuCang->setItem(4,0,new QTableWidgetItem("舱体空闲时是否锁门"));
    ui->tableWidgetFangHuCang->setItem(5,0,new QTableWidgetItem("风扇启动温度"));

    ui->tableWidgetFangHuCang->setItem(0,2,new QTableWidgetItem("检测不到人体后开门等待时间"));
    ui->tableWidgetFangHuCang->setItem(1,2,new QTableWidgetItem("业务办理允许最大时间"));
    ui->tableWidgetFangHuCang->setItem(2,2,new QTableWidgetItem("超时提醒播放时间"));
    ui->tableWidgetFangHuCang->setItem(3,2,new QTableWidgetItem("超时提醒播放间隔"));
    ui->tableWidgetFangHuCang->setItem(4,2,new QTableWidgetItem("防切割报警功能"));
    ui->tableWidgetFangHuCang->setItem(5,2,new QTableWidgetItem(""));
    //设置单元格右对齐
    for(int i=0;i<6;i++)
    {
        ui->tableWidgetFangHuCang->item(i,0)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    }
    for(int i=0;i<6;i++)
    {
        ui->tableWidgetFangHuCang->item(i,2)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    }
    ui->tablewidgetZhaoMing->setColumnCount(2);
    ui->tablewidgetZhaoMing->setRowCount(3);
    ui->tablewidgetZhaoMing->setColumnWidth(0,180);
    ui->tablewidgetZhaoMing->setColumnWidth(1,120);
    ui->tablewidgetZhaoMing->horizontalHeader()->hide();//隐藏表头
    ui->tablewidgetZhaoMing->verticalHeader()->hide();//隐藏行号
    ui->tablewidgetZhaoMing->setItem(0,0,new QTableWidgetItem("模式"));
    ui->tablewidgetZhaoMing->setItem(1,0,new QTableWidgetItem("时间段"));
    ui->tablewidgetZhaoMing->setItem(2,0,new QTableWidgetItem("舱体有人使用即点亮照明灯"));
    for(int i=0;i<3;i++)
    {
        ui->tablewidgetZhaoMing->item(i,0)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    }

    ui->tableWidgetAlarmPara->setColumnCount(2);
    ui->tableWidgetAlarmPara->setRowCount(7);
    ui->tableWidgetAlarmPara->setColumnWidth(0,250);
    ui->tableWidgetAlarmPara->setColumnWidth(1,80);
    ui->tableWidgetAlarmPara->horizontalHeader()->hide();//隐藏表头
    ui->tableWidgetAlarmPara->verticalHeader()->hide();//隐藏行号

    ui->tableWidgetAlarmPara->setItem(0,0,new QTableWidgetItem("是否启用按钮报警"));
    ui->tableWidgetAlarmPara->setItem(1,0,new QTableWidgetItem("是否启用防切割报警"));
    ui->tableWidgetAlarmPara->setItem(2,0,new QTableWidgetItem("是否启用震动报警"));
    ui->tableWidgetAlarmPara->setItem(3,0,new QTableWidgetItem("是否启用烟雾报警"));
    ui->tableWidgetAlarmPara->setItem(4,0,new QTableWidgetItem("是否启用玻璃破碎报警"));
    ui->tableWidgetAlarmPara->setItem(5,0,new QTableWidgetItem("是否启用水浸报警"));
    ui->tableWidgetAlarmPara->setItem(6,0,new QTableWidgetItem("是否启用温度报警"));
    for(int i=0;i<7;i++)
    {
        ui->tableWidgetAlarmPara->item(i,0)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    }
    ui->tableWidgeCjc->setColumnCount(2);
    ui->tableWidgeCjc->setRowCount(7);
    ui->tableWidgeCjc->setColumnWidth(0,140);
    ui->tableWidgeCjc->setColumnWidth(1,90);

    ui->tableWidgeCjc->horizontalHeader()->hide();//隐藏表头
    ui->tableWidgeCjc->verticalHeader()->hide();//隐藏行号
    ui->tableWidgeCjc->setItem(0,0,new QTableWidgetItem("开锁延时"));
    ui->tableWidgeCjc->setItem(1,0,new QTableWidgetItem("设防延时时间"));
    ui->tableWidgeCjc->setItem(2,0,new QTableWidgetItem("刷卡人数"));
    ui->tableWidgeCjc->setItem(3,0,new QTableWidgetItem("进门方式"));
    ui->tableWidgeCjc->setItem(4,0,new QTableWidgetItem("出门方式"));
    ui->tableWidgeCjc->setItem(5,0,new QTableWidgetItem("系统报警状态"));
    ui->tableWidgeCjc->setItem(6,0,new QTableWidgetItem("值守状态"));
    //设置单元格右对齐
    for(int i=0;i<7;i++)
    {
        ui->tableWidgeCjc->item(i,0)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    }
    InitStyle();
    connect(ui->btnOK,SIGNAL(clicked(bool)),this,SLOT(btnOK()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel()));
    connect(ui->btnSetZhaoMingPrar,SIGNAL(clicked(bool)),this,SLOT(btnSetZhaoMingPara()));
    connect(ui->btnSetFangHuCangPara,SIGNAL(clicked(bool)),this,SLOT(btnSetFangHuCangPara()));
    connect(ui->btnSetAlarmPara,SIGNAL(clicked(bool)),this,SLOT(slotbtnSetAlarmPara()));

    connect(ui->btnChangeHySound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeHySound()));
    connect(ui->btnChangeSyzSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangesyzSound()));
    connect(ui->btnChangeWhzSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeWhzSound()));
    connect(ui->btnChangeTimeoutWarnSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeTimeoutWarnSound()));
    connect(ui->btnChangeTimeoutTiShiSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeTimeoutTiShiSound()));
    connect(ui->btnChangeOpenDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeOpenDoorSound()));
    connect(ui->btnChangeOutDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeOutDoorSound()));
    connect(ui->btnChangeFailLockSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeFailLockSound()));
    connect(ui->btnChangeLaDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeLaDoorSound()));

    connect(ui->btnChange0,SIGNAL(clicked(bool)),this,SLOT(slotbtnLed0TextChange()));
    connect(ui->btnChange1,SIGNAL(clicked(bool)),this,SLOT(slotbtnLed1TextChange()));
    connect(ui->btnChange2,SIGNAL(clicked(bool)),this,SLOT(slotbtnLed2TextChange()));
    connect(ui->btnLed0Ok,SIGNAL(clicked(bool)),this,SLOT(slotbtnSetLed0TextOk()));
    connect(ui->btnLed1Ok,SIGNAL(clicked(bool)),this,SLOT(slotbtnSetLed1TextOk()));
    connect(ui->btnLed2Ok,SIGNAL(clicked(bool)),this,SLOT(slotbtnSetLed2TextOk()));
    //connect(frmeditalarmpara,SIGNAL(sigWriteAlarmPara(paraAlarm*)),this,SLOT(slotRefreshUI(paraAlarm*)));
    //connect(frmEditSlavePara,SIGNAL(sigSaveFangHuCangPara(paraFangHuCang*)),this,SLOT(slotSetFangHuCangPara(paraFangHuCang*)));
    //connect(frmEditZhaoMing,SIGNAL(sigSaveZhaoMingPara(paraZhaoMing*)),this,SLOT(slotSetZhaoMingPara(paraZhaoMing*)));
    //connect(frmSelectSound,SIGNAL(sigoperate(QString)),this,SLOT(sloteditedName(QString)));
    connect(ui->btnSetCjcPara,SIGNAL(clicked(bool)),this,SLOT(slotShowSetCjcParaDialog()));
}

FrmModulePara::~FrmModulePara()
{
    delete ui;
}
void FrmModulePara::InitStyle()
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
    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

bool FrmModulePara::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void FrmModulePara::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void FrmModulePara::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void FrmModulePara::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void FrmModulePara::on_btnMenu_Close_clicked()
{
    this->close();
}

void FrmModulePara::on_btnMenu_Max_clicked()
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

void FrmModulePara::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}
void FrmModulePara::btnOK()
{
    pModule->name = ui->leName->text();
    emit sigSaveModulePara(pModule);
    this->close();
}
void FrmModulePara::btnCancel()
{
    this->close();
}
void FrmModulePara::SetTitleText(QString title)
{
    ui->lab_Title->setText(title);
}
void FrmModulePara::DisplaySoundName()
{

    //获取声音名称
    ui->hsVoice->setValue(pdat->soundVolume);
    ui->le0->setText(pdat->soundName[0]);
    ui->le1->setText(pdat->soundName[1]);
    ui->le2->setText(pdat->soundName[2]);
    ui->le3->setText(pdat->soundName[3]);
    ui->le4->setText(pdat->soundName[4]);
    ui->le5->setText(pdat->soundName[5]);
    ui->le6->setText(pdat->soundName[6]);
    ui->le7->setText(pdat->soundName[7]);
    ui->le8->setText(pdat->soundName[8]);
}
void FrmModulePara::DisplayZhaoMingPara()
{
    ui->tablewidgetZhaoMing->setItem(0,1,new QTableWidgetItem(pdat->zhaoMing.getModel()));
    QString temp =QString::number(pdat->zhaoMing.startHour)+"点"+QString::number(pdat->zhaoMing.startMinute)+"分"+"到"+QString::number(pdat->zhaoMing.endHour)+"点"+QString::number(pdat->zhaoMing.endMinute)+"分";
    ui->tablewidgetZhaoMing->setItem(1,1,new QTableWidgetItem(temp));
    ui->tablewidgetZhaoMing->setItem(2,1,new QTableWidgetItem(pdat->zhaoMing.manEnable?"启用":"不启用"));
}
void FrmModulePara::DisplayFangHuCangPara()
{
    switch (model)
    {
    case 0://防护舱
    {
        ui->tableWidgetFangHuCang->setItem(0,1,new QTableWidgetItem(pdat->fangHuCang.getlockModel()));
        ui->tableWidgetFangHuCang->setItem(1,1,new QTableWidgetItem(QString::number(pdat->fangHuCang.openLockTime)+"秒钟"));
        ui->tableWidgetFangHuCang->setItem(2,1,new QTableWidgetItem(pdat->fangHuCang.signalModel?"常开":"常闭"));
        ui->tableWidgetFangHuCang->setItem(3,1,new QTableWidgetItem(pdat->fangHuCang.peopleEquModel?"被动":"主动"));
        ui->tableWidgetFangHuCang->setItem(4,1,new QTableWidgetItem(pdat->fangHuCang.kongCangLockorNot?"是":"否"));
        ui->tableWidgetFangHuCang->setItem(5,1,new QTableWidgetItem(QString::number(pdat->fangHuCang.fanRunTemperature)+"度"));

        ui->tableWidgetFangHuCang->setItem(0,3,new QTableWidgetItem(QString::number(pdat->fangHuCang.noManOpenLockTime)+"秒钟"));
        ui->tableWidgetFangHuCang->setItem(1,3,new QTableWidgetItem(QString::number(pdat->fangHuCang.OptBussinessTime)+"分钟"));
        ui->tableWidgetFangHuCang->setItem(2,3,new QTableWidgetItem(QString::number(pdat->fangHuCang.timeOutRemind)+"秒钟"));
        ui->tableWidgetFangHuCang->setItem(3,3,new QTableWidgetItem(QString::number(pdat->fangHuCang.warnningDelayTime)+"秒钟"));
        ui->tableWidgetFangHuCang->setItem(4,3,new QTableWidgetItem(pdat->fangHuCang.fangQiewarnning?"启动":"不启用"));
    }
        break;
    case 1://加钞间
    {
        ui->tableWidgeCjc->setItem(0,1,new QTableWidgetItem(QString::number(pdat->fangHuCang.openLockTime)+"秒钟"));
        ui->tableWidgeCjc->setItem(1,1,new QTableWidgetItem(QString::number(pdat->fangHuCang.setGuardDelayTime)+"秒钟"));
        ui->tableWidgeCjc->setItem(2,1,new QTableWidgetItem(QString::number(pdat->fangHuCang.userNum)+"+1"));
        ui->tableWidgeCjc->setItem(3,1,new QTableWidgetItem(pdat->fangHuCang.getInDoorModel()));
        ui->tableWidgeCjc->setItem(4,1,new QTableWidgetItem(pdat->fangHuCang.getOutDoorModel()));
        ui->tableWidgeCjc->setItem(5,1,new QTableWidgetItem(pdat->fangHuCang.systemAlarmStatus?"设防":"撤防"));
        ui->tableWidgeCjc->setItem(6,1,new QTableWidgetItem(pdat->fangHuCang.isMonitorOrNot?"有人值守":"无人值守"));
    }
        break;
    case 0xff://bootloader
        break;
    }
}
void FrmModulePara::DisplayAlarmPara()
{
    ui->tableWidgetAlarmPara->setItem(0,1,new QTableWidgetItem(pdat->alarmPara.btnAlarmEnable?"启用":"不启用"));
    ui->tableWidgetAlarmPara->setItem(1,1,new QTableWidgetItem(pdat->alarmPara.cutAlarmEnable?"启用":"不启用"));
    ui->tableWidgetAlarmPara->setItem(2,1,new QTableWidgetItem(pdat->alarmPara.zhengDongAlarmEnable?"启用":"不启用"));
    ui->tableWidgetAlarmPara->setItem(3,1,new QTableWidgetItem(pdat->alarmPara.yanWuAlarmEnable?"启用":"不启用"));
    ui->tableWidgetAlarmPara->setItem(4,1,new QTableWidgetItem(pdat->alarmPara.boLiAlarmEnable?"启用":"不启用"));
    ui->tableWidgetAlarmPara->setItem(5,1,new QTableWidgetItem(pdat->alarmPara.shuiQinAlarmEnable?"启用":"不启用"));
    ui->tableWidgetAlarmPara->setItem(6,1,new QTableWidgetItem(pdat->alarmPara.tempAlarmEnable?"启用":"不启用"));
}
void FrmModulePara::setModel(quint8 mod)
{
    model=mod;
    switch(mod)
    {
    case 0://防护舱
    {
        ui->tabWidgetSlavePara->clear();
        ui->tabWidgetSlavePara->addTab(ui->tabSlavePara,"控制参数");
        ui->tabWidgetSlavePara->addTab(ui->tabVoicePara,"语音状态");
        ui->tabWidgetSlavePara->addTab(ui->tabLedPara,"LED参数");
        ui->tabWidgetSlavePara->addTab(ui->tabAlarmPara,"报警参数");
        //语音界面更设置
        ui->labelVoice_1->setText("拉门提示语音内容：");
        ui->labelVoice_2->setText("使用中语音内容：");
        ui->labelVoice_3->setText("欢迎语音内容：");
        ui->labelVoice_4->setText("未锁好提示语音内容：");
        ui->labelVoice_5->setText("超时提醒语音内容：");
        ui->labelVoice_6->setText("超时提示语音内容：");
        ui->labelVoice_7->setText("门开提示语音内容：");
        ui->labelVoice_8->setText("再见语音内容：");
        ui->labelVoice_9->setText("维护中语音内容：");
    }
        break;
    case 1://加钞间
    {
        ui->tabWidgetSlavePara->clear();
        ui->tabWidgetSlavePara->addTab(ui->tabSlaveJCJPara,"控制参数");
        ui->tabWidgetSlavePara->addTab(ui->tabVoicePara,"语音状态");
        ui->tabWidgetSlavePara->addTab(ui->tabLedPara,"LED参数");
        ui->tabWidgetSlavePara->addTab(ui->tabAlarmPara,"报警参数");

        //语音界面更设置
        ui->labelVoice_1->setText("删除提示语音内容：");
        ui->labelVoice_2->setText("请刷卡语音内容：");
        ui->labelVoice_3->setText("欢迎语音内容：");
        ui->labelVoice_4->setText("未锁好提示语音内容：");
        ui->labelVoice_5->setText("设防语音内容：");
        ui->labelVoice_6->setText("撤防语音内容：");
        ui->labelVoice_7->setText("注册成功语音内容：");
        ui->labelVoice_8->setText("再见语音内容：");
        ui->labelVoice_9->setText("主卡语音内容：");
    }
        break;
    default:

        break;
    }
}
//mod-0新建 名称可以编辑 1－修改模板 名称不可以修改
void FrmModulePara::SetPara(QString name, paraModule *p_Module, quint8 mod)
{
    pModule=p_Module;
    ui->leName->setText(name);
    switch(mod)
    {
    case 0:
        ui->leName->setReadOnly(false);
        break;
    case 1:
        ui->leName->setReadOnly(true);
        break;
    default:
        ui->leName->setReadOnly(false);
        break;
    }
    if(&(pModule->pdat))
    {
        pdat = &pModule->pdat;
        emit sigUpdateSoundName(pdat);
        DisplayZhaoMingPara();
        DisplayFangHuCangPara();
        DisplayAlarmPara();
        DisplaySoundName();
        ui->txtBrowse0->setText(pdat->ledText[0]);
        ui->txtBrowse1->setText(pdat->ledText[1]);
        ui->txtBrowse2->setText(pdat->ledText[2]);
    }
}
void FrmModulePara::setVoiceModel(QSqlTableModel *model)
{
    voiceModel = model;
}
void FrmModulePara::btnSetFangHuCangPara()
{
    if(pdat!=NULL)
    {
        frmEditParamater *frmEditSlavePara = new frmEditParamater();
        frmEditSlavePara->initPara(pdat->fangHuCang,1);
        connect(frmEditSlavePara,SIGNAL(sigSaveFangHuCangPara(paraFangHuCang*)),this,SLOT(slotSetFangHuCangPara(paraFangHuCang*)));
        frmEditSlavePara->exec();
        //delete frmEditSlavePara;
    }
    else
    {
        //弹出错误提示
    }
}

void FrmModulePara::btnSetZhaoMingPara()
{
    if(pdat!=NULL)
    {
        frmEditZhaoMingPara *frmEditZhaoMing = new frmEditZhaoMingPara();
        frmEditZhaoMing->initPara(pdat->zhaoMing,1);
        connect(frmEditZhaoMing,SIGNAL(sigSaveZhaoMingPara(paraZhaoMing*)),this,SLOT(slotSetZhaoMingPara(paraZhaoMing*)));
        frmEditZhaoMing->exec();
        //delete frmEditZhaoMing;
    }
    else
    {
        //弹出错误提示
    }
}

void FrmModulePara::slotbtnSetAlarmPara()
{
    if(pdat!=NULL)
    {
        frmEditAlarmPara *frm=new frmEditAlarmPara();
        connect(frm,SIGNAL(sigWriteAlarmPara(paraAlarm*)),this,SLOT(slotRefreshUI(paraAlarm*)));
        frm->initPara(pdat->alarmPara, 1);
        frm->exec();
        //delete frm;
    }
    else
    {
        //弹出错误提示
    }
}
void FrmModulePara::slotSetZhaoMingPara(paraZhaoMing * data)
{
    pdat->zhaoMing = *data;
    DisplayZhaoMingPara();
}
void FrmModulePara::slotSetFangHuCangPara(paraFangHuCang * data)
{
    pdat->fangHuCang = *data;
    DisplayFangHuCangPara();
}
void FrmModulePara::slotSetSound(quint8 id,QString md5)
{
    QByteArray byteMd5 = myHelper::hexStringtoByteArray(md5);
    pdat->soundMd5[id]=byteMd5;
    emit sigUpdateSoundName(pdat);
    DisplaySoundName();
}
void FrmModulePara::changeVoice(quint8 id)
{
    editedName="";
    FrmSelectSound *frm=new FrmSelectSound();
    connect(frm,SIGNAL(sigoperate(QString)),this,SLOT(sloteditedName(QString)));
    frm->setTreeViewModel(voiceModel);
    frm->exec();
    //delete frm;
    if(editedName!="")
    {
        QString fileName="";
        QString md5="";
        //读取文件的名称和md5
        findfileName(fileName,md5);
        pdat->soundName[id] = editedName;
        slotSetSound(id,md5);

    }
}
void FrmModulePara::slotBtnChangeHySound()
{
    //emit sigChangModuleSound(0);
    changeVoice(0);
}

void FrmModulePara::slotBtnChangesyzSound()
{
    //emit sigChangModuleSound(1);
    changeVoice(1);
}
void FrmModulePara::slotBtnChangeWhzSound()
{
    //emit sigChangModuleSound(2);
    changeVoice(2);
}
void FrmModulePara::slotBtnChangeTimeoutWarnSound()
{
    //emit sigChangModuleSound(3);
    changeVoice(3);
}
void FrmModulePara::slotBtnChangeTimeoutTiShiSound()
{
    //emit sigChangModuleSound(4);
    changeVoice(4);
}
void FrmModulePara::slotBtnChangeOutDoorSound()
{
    //emit sigChangModuleSound(5);
    changeVoice(5);
}
void FrmModulePara::slotBtnChangeOpenDoorSound()
{
    //emit sigChangModuleSound(6);
    changeVoice(6);
}
void FrmModulePara::slotBtnChangeFailLockSound()
{
    //emit sigChangModuleSound(7);
    changeVoice(7);
}
void FrmModulePara::slotBtnChangeLaDoorSound()
{
    //emit sigChangModuleSound(8);
    changeVoice(8);
}

void FrmModulePara::on_hsVoice_valueChanged(int value)
{
    QString strValue = QString::number(value);
    ui->lbVoice->setText(strValue);
    pdat->soundVolume = ui->hsVoice->value();
}
//led欢迎内容修改
void FrmModulePara::slotbtnLed0TextChange()
{
    if(ui->txtBrowse0->isReadOnly())
    {
        ui->txtBrowse0->setReadOnly(false);
        ui->btnChange0->setText("取消");
        ui->btnLed0Ok->setVisible(true);
    }
    else
    {
        ui->txtBrowse0->setText(pdat->ledText[0]);
        ui->btnChange0->setText("更换");
        ui->txtBrowse0->setReadOnly(true);
        ui->btnLed0Ok->setVisible(false);
    }
}
//确认led欢迎内容修改
void FrmModulePara::slotbtnSetLed0TextOk()
{
    ui->btnChange0->setText("更换");
    ui->txtBrowse0->setReadOnly(true);
    ui->btnLed0Ok->setVisible(false);
    pdat->ledText[0] = ui->txtBrowse0->toPlainText();
}
//led使用中内容修改
void FrmModulePara::slotbtnLed1TextChange()
{
    if(ui->txtBrowse1->isReadOnly())
    {
        ui->txtBrowse1->setReadOnly(false);
        ui->btnChange1->setText("取消");
        ui->btnLed1Ok->setVisible(true);
    }
    else
    {
        ui->txtBrowse1->setText(pdat->ledText[1]);
        ui->btnChange1->setText("更换");
        ui->txtBrowse1->setReadOnly(true);
        ui->btnLed1Ok->setVisible(false);
    }
}
//确认led使用中内容修改
void FrmModulePara::slotbtnSetLed1TextOk()
{
    ui->btnChange1->setText("更换");
    ui->txtBrowse1->setReadOnly(true);
    ui->btnLed1Ok->setVisible(false);
    pdat->ledText[1] = ui->txtBrowse1->toPlainText();
}
//led暂停使用内容修改
void FrmModulePara::slotbtnLed2TextChange()
{
    if(ui->txtBrowse2->isReadOnly())
    {
        ui->txtBrowse2->setReadOnly(false);
        ui->btnChange2->setText("取消");
        ui->btnLed2Ok->setVisible(true);
    }
    else
    {
        ui->txtBrowse2->setText(pdat->ledText[2]);
        ui->btnChange2->setText("更换");
        ui->txtBrowse2->setReadOnly(true);
        ui->btnLed2Ok->setVisible(false);
    }
}
//确认led暂停使用内容修改
void FrmModulePara::slotbtnSetLed2TextOk()
{
    ui->btnChange2->setText("更换");
    ui->txtBrowse2->setReadOnly(true);
    ui->btnLed2Ok->setVisible(false);
    pdat->ledText[2] = ui->txtBrowse2->toPlainText();
}
//
void FrmModulePara::slotRefreshUI(paraAlarm* par)
{
    pdat->alarmPara = *par;
    DisplayAlarmPara();
}
void FrmModulePara::sloteditedName(QString name)
{
    editedName = name;
}
void FrmModulePara::findfileName(QString &fileName,QString &md5)
{
    fileName = "";
    md5 = "";
    int num = voiceLibrary.length();
    for(int i=0;i<num;i++)
    {
        if(voiceLibrary.at(i).name == editedName)
        {
            fileName = voiceLibrary.at(i).fileName;
            md5 = voiceLibrary.at(i).md5;
        }
    }
}
void FrmModulePara::slotShowSetCjcParaDialog()
{
   if(pdat!=NULL)
   {
       //frmEditParamater *frmEditSlavePara = new frmEditParamater();
       //frmEditSlavePara->initPara(pdat->fangHuCang,1);
       //connect(frmEditSlavePara,SIGNAL(sigSaveFangHuCangPara(paraFangHuCang*)),this,SLOT(slotSetFangHuCangPara(paraFangHuCang*)));
       //frmEditSlavePara->exec();
       //delete frmEditSlavePara;
       FrmEditCJCWorkPara *frmEditCjcWorkPara = new FrmEditCJCWorkPara();
       frmEditCjcWorkPara->initPara(pdat->fangHuCang,0);
       connect(frmEditCjcWorkPara,SIGNAL(sigWriteCjcWorkPara(paraFangHuCang*)),this,SLOT(slotSetJCJPara(paraFangHuCang*)));
       frmEditCjcWorkPara->setModal(true);
       frmEditCjcWorkPara->show();
   }
   else
   {
       //弹出错误提示
   }
}
void FrmModulePara::slotSetJCJPara(paraFangHuCang * data)
{
    pdat->fangHuCang = *data;
    DisplayFangHuCangPara();
}
