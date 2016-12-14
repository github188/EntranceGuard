#include "frmmain.h"
#include "ui_frmmain.h"
#include "iconhelper.h"
#include "../myhelper.h"
#include "datastruct.h"
#include <qtextcodec.h>
#include "qstring.h"
#include <QString>
#include <qglobal.h>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include "../TreeViewClass/itemdelegate.h"
#include "../TreeViewClass/treeitem.h"
#include "../TreeViewClass/treemodel.h"
#include <QtSql>
#include "frmoperateparentmsgbox.h"
#include "frmreadslavemsg.h"
#include "../DATA/slavemsg.h"
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>
#include "./AppConfig/myapp.h"
#include <QSettings>

QList<VoiceInfoBank> voiceLibrary;
QString sFilePath;
QString frmMain::ConnectVersion = "2.7,2.8";

frmMain::frmMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    sFilePath = QApplication::applicationDirPath()+"/Voice/";
    comBuffer = new CCycleBuffer(1024*100);
    //mMediaObj = Phonon::createPlayer(Phonon::MusicCategory);
    mMediaObj =new Phonon::MediaObject();
    audioOutput = new Phonon::AudioOutput();
    Phonon::createPath(mMediaObj,audioOutput);
    myHelper::FormInCenter(this);
    searchTimeOut = new QTimer(this);
    searchTime = new QTimer(this);
    udpClient = new QUdpSocket(this);
    //实例化QTcpServer服务器，进行监听，等待客户端的连接
    tcpServer = new myTcpServer(this);
    windowStatus=0;
    //开始监听
    bool ok=tcpServer->listen(QHostAddress::Any,13710);
    //tcpClient = new QTcpSocket(this);
    //用在多客户端监听同一个服务器端口等时特别有效)，和ReuseAddressHint模式(重新连接服务器)
    //udpClient->bind(myApp::UdpPort.toInt());
    iconConnected = QIcon(":/image/slave_online.ico");
    iconDisConnected = QIcon(":/image/slave_offline.ico");
    iconConnecting = QIcon(":/image/slave_connecting.ico");
    gateOpen = "border-image:url(:/设备状态图标/image/设备状态图标/门-打开.png);border:0px;";
    gateClose = "border-image:url(:/设备状态图标/image/设备状态图标/门-关闭.png);border:0px;";
    fanClose =  "border-image:url(:/使用状态图标/image/使用状态图标/风扇-关闭.png);border:0px;";
    warning = "border-image:url(:/状态图标/image/图标/报警.png);border:0px;";

    statusNull = "border-image:url(:/使用状态图标/image/使用状态图标/未知状态.png);border:0px;";
    doorStatusNull = statusNull; //"border-image:url(:/设备状态图标/image/设备状态图标/门-未知.png);border:0px;";
    openLock = "border-image:url(:/使用状态图标/image/使用状态图标/门-解锁.png);border:0px;";
    closeLock = "border-image:url(:/使用状态图标/image/使用状态图标/门-锁闭.png);border:0px;";
    //使用状态图标
    usingGif = new QMovie(":/使用状态图标/image/使用状态图标/使用状态-使用.gif");
    notUsingIcon = "border-image:url(:/使用状态图标/image/使用状态图标/使用状态-空闲.png);border:0px;";
    //照明状态图标
    zhaoMingOpen = "border-image:url(:/使用状态图标/image/使用状态图标/照明灯-开.png);border:0px;";
    zhaoMingClose = "border-image:url(:/使用状态图标/image/使用状态图标/照明灯-关.png);border:0px;";
    //灯箱状态图标
    dengXiangOpen= "border-image:url(:/使用状态图标/image/使用状态图标/灯箱-开.png);border:0px;";
    dengXiangClose = "border-image:url(:/使用状态图标/image/使用状态图标/灯箱-关.png);border:0px;";
    //灯箱状态图标
    treeViewContextMenu = NULL;
    currentSlave = NULL;
    //报警状态图标
    qieGeAlarm = "border-image:url(:/报警状态图标/image/报警状态图标/切割报警-报警.png);border:0px;";
    qieGeNormal = "border-image:url(:/报警状态图标/image/报警状态图标/切割报警-正常.png);border:0px;";
    baoJingDisable = "border-image:url(:/报警状态图标/image/报警状态图标/报警-未启用.png);border:0px;";
    btnAlarm = "border-image:url(:/报警状态图标/image/报警状态图标/按钮报警-报警.png);border:0px;";
    btnNormal = "border-image:url(:/报警状态图标/image/报警状态图标/按钮报警-正常.png);border:0px;";
    //震动报警
    zhenDongAlarm = "border-image:url(:/报警状态图标/image/报警状态图标/震动报警-报警.png);border:0px;";
    zhenDongNormal = "border-image:url(:/报警状态图标/image/报警状态图标/震动报警-正常.png);border:0px;";
    //烟雾报警
    yanWuAlarm = "border-image:url(:/报警状态图标/image/报警状态图标/烟感报警-报警.png);border:0px;";
    yanWuNormal = "border-image:url(:/报警状态图标/image/报警状态图标/烟感报警-正常.png);border:0px;";
    //温度报警
    tempAlarm = "border-image:url(:/报警状态图标/image/报警状态图标/温度报警-报警.png);border:0px;";
    tempNormal = "border-image:url(:/报警状态图标/image/报警状态图标/温度报警-正常.png);border:0px;";
    //玻璃破碎报警
    boLiAlarm = "border-image:url(:/报警状态图标/image/报警状态图标/玻璃破碎报警-报警.png);border:0px;";
    boLiNormal = "border-image:url(:/报警状态图标/image/报警状态图标/玻璃破碎报警-正常.png);border:0px;";
    //水浸报警
    shuiQinAlarm = "border-image:url(:/报警状态图标/image/报警状态图标/水位报警-报警.png);border:0px;";
    shuiQinNormal = "border-image:url(:/报警状态图标/image/报警状态图标/水位报警-正常.png);border:0px;";

    fangHuCangUse = "border-image:url(:/使用状态图标/image/使用状态图标/门-启用.png);border:0px;";
    fangHuCangNotUse = "border-image:url(:/使用状态图标/image/使用状态图标/门-禁用.png);border:0px;";
    /**************加钞间状态图标**************************/
    //加钞间状态图标
    iconCjcWorkNormalState = "border-image:url(:/加钞间图标/image/加钞间状态图标/工作状态-正常.gif);border:0px;";//工作状态－正常
    iconCjcWorkIsGuardingState = "border-image:url(:/加钞间图标/image/加钞间状态图标/工作状态-正常有人值守.gif);border:0px;";//工作状态－正常 有人值守
    iconCjcWorkOperateMainCardState = "border-image:url(:/加钞间图标/image/加钞间状态图标/工作状态-主卡注册、删除状态.png);border:0px;";//工作状态－主卡添加，删除
    iconCjcWorkEditAdminState = "border-image:url(:/加钞间图标/image/加钞间状态图标/修改管理员.png);border:0px;";//工作状态－修改管理员

    iconCjcAlarmSetGuardState = "border-image:url(:/加钞间图标/image/加钞间状态图标/报警状态-设防.png);border:0px;";//报警状态－设防
    iconCjcAlarmWaitSetGuardState = "border-image:url(:/加钞间图标/image/加钞间状态图标/报警状态-等待设防.png);border:0px;";//报警状态－等待设防
    iconCjcAlarmResetGuardState = "border-image:url(:/加钞间图标/image/加钞间状态图标/报警状态-撤防.png);border:0px;";//报警状态－撤防

    iconCjcAlarmIllegalOpenDoorEnable = "border-image:url(:/加钞间图标/image/加钞间状态图标/非法开门报警-报警.png);border:0px;";//非法开门报警状态－报警
    iconCjcAlarmIllegalOpenDoorDisable = "border-image:url(:/加钞间图标/image/加钞间状态图标/非法开门报警-正常.png);border:0px;";//非法开门报警状态－正常

    iconCjcAlarmIllegalInEnable = "border-image:url(:/加钞间图标/image/加钞间状态图标/非法入侵报警-报警.png);border:0px;";//非法入侵报警状态－报警
    iconCjcAlarmIllegalInDisable = "border-image:url(:/加钞间图标/image/加钞间状态图标/非法入侵-正常.png);border:0px;";//非法入侵报警状态－正常
    /**************加钞间状态图标结束**************************/
    selectedParentName = "";
    movie = new QMovie(":/使用状态图标/image/使用状态图标/风扇-打开.gif");
    connectingGif = new QMovie(":/联网状态图标/image/联网状态图标/连接中.gif");
    select_Parent_sql = "select * from TZone";
    select_macParent_sql = "SELECT ZName FROM TBelong WHERE Mac=";
    select_indexParent_sql = "SELECT IndexInTree FROM TZone WHERE ZName=";
    select_TSlave_sql = "select  * from TSlave";
    select_TParamater_sql = "SELECT * FROM TParamater WHERE Mac=:mac";
    select_TVoice_sql = "SELECT * FROM TVoice";
    select_TMparamater_sql = "SELECT * FROM TParamater";
    select_Module_sql = "SELECT * FROM TModule";
    select_TNetWork_sql = "SELECT * FROM SlaveNetWork WHERE Mac=:mac";
    select_TUser_sql = "SELECT * FROM TPermit";

    insert_TZone_sql = "insert into TZone values (?, ?)";
    insert_TSlave_sql = "insert into TSlave values (?,?,?,?,?,?,?)";
    insert_TParamater_sql = "insert into TParamater (Mac,OpenLockTime,OperationTime,RemindTimeout,"
                            "Interval,NoManOpenTime,OpenFanTemp,HumanDetectingInputType,"
                            "HumanDetectingType,LockType,LockWhileIdle,FangQiewarning,"
                            "Model,StartHour,StartMinute,StopHour,StopMinute,ExistManUseLine,"
                            "LPMWelcome,LPMUsing,LPMMaintaining,"
                            "Volume,Welcome,Use,Maintaining,Reminding,Timeout,Goodbye,DoorOpen,NotLocked,SlideDoor,"
                            "btnAlarmEnable,cutAlarmEnable,zhengDongAlarmEnable,yanWuAlarmEnable,"
                            "boLiAlarmEnable,shuiQinAlarmEnable,tempAlarmEnable,userNum,systemAlarmStatus,setGuardDelayTime,"
                            "isMonitorOrNot,inDoorModel,outDoorModel,doorCiAlarmEnable,existManAlarmEnable)"
                            " values "
                            "(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    insert_TMparamater_sql = "insert into TMparamater (Name,OpenLockTime,OperationTime,RemindTimeout,"
                         "Interval,NoManOpenTime,OpenFanTemp,HumanDetectingInputType,"
                         "HumanDetectingType,LockType,LockWhileIdle,FangQiewarning,"
                         "Model,StartHour,StartMinute,StopHour,StopMinute,ExistManUseLine,"
                         "LPMWelcome,LPMUsing,LPMMaintaining,"
                         "Volume,Welcome,Use,Maintaining,Reminding,Timeout,Goodbye,DoorOpen,NotLocked,SlideDoor,"
                         "btnAlarmEnable,cutAlarmEnable,zhengDongAlarmEnable,yanWuAlarmEnable,"
                         "boLiAlarmEnable,shuiQinAlarmEnable,tempAlarmEnable,userNum,systemAlarmStatus,setGuardDelayTime,"
                         "isMonitorOrNot,inDoorModel,outDoorModel,doorCiAlarmEnable,existManAlarmEnable)"
                         " values "
                         "(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    insert_TBelong_sql = "insert into TBelong values (?,?)";
    insert_TVoice_sql = "insert into TVoice values (?,?,?,?)";
    insert_TModule_sql = "insert into TModule values (?,?)";
    insert_TNetWork_sql = "insert into SlaveNetWork values (?,?,?,?,?,?)";

    delete_TSlave_sql = "delete from TSlave where Mac = ?";
    delete_TZone_sql = "delete from TZone where ZName = ?";
    delete_TBelong_sql = "delete from TBelong where Mac = ?";
    delete_TParamater_sql = "delete from TParamater where Mac = ?";
    delete_TVoice_sql = "delete from TVoice where Name = ?";
    delete_TMparamater_sql = "delete from TMparamater where Name = ?";
    delete_TModule_sql = "delete from TModule where Name = ?";
    delete_TNetWork_sql = "delete from SlaveNetWork where Mac = ?";

    update_TSlave_index_sql = "update TSlave set IndexInTree = :index where Mac = :name";
    update_TSlave_sql = "update TSlave set Type = :type,Version = :version, Name = :name,Ip = :ip where Mac = :mac";
    update_TZone_index_sql = "update TZone set IndexInTree = :index where ZName = :name";
    update_TZone_name_sql = "update TZone set ZName = :name where IndexInTree = :index";
    update_TBelong_ZName_sql = "update TBelong set ZName = :name where Mac = :mac";
    update_TPara_sql = "update TParamater set OpenLockTime=:OpenLockTime,"
                       "OperationTime=:OperationTime, RemindTimeout=:RemindTimeout,"
                       "Interval=:Interval,NoManOpenTime=:NoManOpenTime,"
                       "OpenFanTemp=:OpenFanTemp,HumanDetectingInputType=:HumanDetectingInputType,"
                       "HumanDetectingType=:HumanDetectingType,LockType=:LockType,"
                       "LockWhileIdle=:LockWhileIdle,FangQiewarning=:FangQiewarning,"
                       "Model=:Mode,StartHour=:startHour,StartMinute=:startMinute,"
                       "StopHour=:stopHour,StopMinute=:stopMinute,ExistManUseLine=:manEnable,"
                       "LPMWelcome=:lpmWelcome,LPMUsing=:lpmUsing,LPMMaintaining=:lpmMaintaining,"
                       "Volume=:volume,Welcome=:welcome,Maintaining=:maintaining,"
                       "Reminding=:reminding,Timeout=:timeout,Goodbye=:goodbye,DoorOpen=:doorOpen,"
                       "NotLocked=:notLocked,Use=:using,SlideDoor=:slideDoor,"
                       "btnAlarmEnable=:btnAlarmEnable,cutAlarmEnable=:cutAlarmEnable,zhengDongAlarmEnable=:zhengDongAlarmEnable,yanWuAlarmEnable=:yanWuAlarmEnable,"
                       "boLiAlarmEnable=:boLiAlarmEnable,shuiQinAlarmEnable=:shuiQinAlarmEnable,tempAlarmEnable=:tempAlarmEnable,"
                       "userNum=:userNum,systemAlarmStatus=:systemAlarmStatus,setGuardDelayTime=:setGuardDelayTime,"
                       "isMonitorOrNot=:isMonitorOrNot,inDoorModel=:inDoorModel,outDoorModel=:outDoorModel,"
                       "doorCiAlarmEnable=:doorCiAlarmEnable,existManAlarmEnable=:existManAlarmEnable"
                       " where Mac = :mac";
    update_TVoice_sql = "update TVoice set Md5=:md5,FileName=:fileName where Name = :name";
    update_TMparamater_sql = "update TMparamater set OpenLockTime=:OpenLockTime,"
                       "OperationTime=:OperationTime, RemindTimeout=:RemindTimeout,"
                       "Interval=:Interval,NoManOpenTime=:NoManOpenTime,"
                       "OpenFanTemp=:OpenFanTemp,HumanDetectingInputType=:HumanDetectingInputType,"
                       "HumanDetectingType=:HumanDetectingType,LockType=:LockType,"
                       "LockWhileIdle=:LockWhileIdle,FangQiewarning=:FangQiewarning,"
                       "Model=:Mode,StartHour=:startHour,StartMinute=:startMinute,"
                       "StopHour=:stopHour,StopMinute=:stopMinute,ExistManUseLine=:manEnable,"
                       "LPMWelcome=:lpmWelcome,LPMUsing=:lpmUsing,LPMMaintaining=:lpmMaintaining,"
                       "Volume=:volume,Welcome=:welcome,Maintaining=:maintaining,"
                       "Reminding=:reminding,Timeout=:timeout,Goodbye=:goodbye,DoorOpen=:doorOpen,"
                       "NotLocked=:notLocked,Use=:using,SlideDoor=:slideDoor,"
                       "btnAlarmEnable=:btnAlarmEnable,cutAlarmEnable=:cutAlarmEnable,zhengDongAlarmEnable=:zhengDongAlarmEnable,yanWuAlarmEnable=:yanWuAlarmEnable,"
                       "boLiAlarmEnable=:boLiAlarmEnable,shuiQinAlarmEnable=:shuiQinAlarmEnable,tempAlarmEnable=:tempAlarmEnable,"
                       "userNum=:userNum,systemAlarmStatus=:systemAlarmStatus,setGuardDelayTime=:setGuardDelayTime,"
                       "isMonitorOrNot=:isMonitorOrNot,inDoorModel=:inDoorModel,outDoorModel=:outDoorModel,"
                       "doorCiAlarmEnable=:doorCiAlarmEnable,existManAlarmEnable=:existManAlarmEnable"
                       " where Name = :name";
    update_TModule_sql = "update TModule set Name=:name where level = 2";
    update_TPuser_sql = "update TPermit set permitEditSystemPar=:permitEditSystemPar,"
                       "permitEditAreaInfo=:permitEditAreaInfo, permitRegFangHuCang=:permitRegFangHuCang,"
                       "permitLogoutFangHuCang=:permitLogoutFangHuCang,permitEditSoundLiabrary=:permitEditSoundLiabrary,"
                       "permitEditModule=:permitEditModule,permitEditFangHuCangPar=:permitEditFangHuCangPar,"
                       "permitEditZhaoMingPar=:permitEditZhaoMingPar,permitEditVoicePar=:permitEditVoicePar,"
                       "permitAdjustTime=:permitAdjustTime,permitOpenDoor=:permitOpenDoor,"
                       "permitUseorNotusedFangHuCang=:permitUseorNotusedFangHuCang,permitLockorOpenFangHuCang=:permitLockorOpenFangHuCang,"
                       "permitEditLedText=:permitEditLedText,permitChangeAdmin=:permitChangeAdmin"
                       " where Level = :Level";

    model = new TreeModel(this);
    slaveListModel = new ListModel(this);
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(myApp::AppPath+"table.db");

    //打开数据库
    if(!database.open())
    {
        QLOG_FATAL() <<database.lastError();
        myHelper::ShowMessageBoxError("数据库文件打开错误！");
        this->close();
    }
    query = new QSqlQuery(database);
    systemLogquery = new QSqlQuery(database);
    slaveLogquery = new QSqlQuery(database);
    treeViewParentTbModel = new QSqlTableModel(this, database);

    voiceModel = new QSqlTableModel(this,database);
    moduleModel = new QSqlTableModel(this,database);

    treeViewParentTbModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置为手动保存后才会真正写入数据库

    searchEquForm = new frmSearchEquipment();
    addEquForm = new frmAddSlave();
    frmSelectParentInTree = new SelectParentInTree();
    //frmOperateParent = new frmOperateParentInTree();
    frmOperateItemBox = new FrmOperateParentMsgBox();
    frmSaveSound = new FrmSaveSound();
    //frmEditSlavePara = new frmEditParamater();
    //frmEditZhaoMing = new frmEditZhaoMingPara();
    frmVoiceManager = new FrmManagerVoice();
    frmReadSoundProcess = new FrmReadSoundProcess();
    //frmSelectSound = new FrmSelectSound();
    //frmAddSound = new FrmAddSound();
    frmManagerModel = new FrmManagerModule();
    frmUseModule = new FrmUseModule();
    //frmModulePara = new FrmModulePara();
    frmAlarm = new frmAlarmBox();
    //frmEditAlarm = new frmEditAlarmPara();
    frmEditUserPwd = new FrmEditUserPwd();
    frmInfo = new frmSoftwareInfo();
    frmEditUserPermission = new frmGuestPermissionConfig();

    ui->btnLed0Ok->setVisible(false);
    ui->btnLed1Ok->setVisible(false);
    ui->btnLed2Ok->setVisible(false);
    connect(this,SIGNAL(sigAnalysisData()),this,SLOT(slotAnalysisData()));
    connect(udpClient,SIGNAL(readyRead()),this,SLOT(readPacketage()));
    connect(searchTimeOut,SIGNAL(timeout()),this,SLOT(slotEmitSearchDone()));
    connect(searchTime,SIGNAL(timeout()),this,SLOT(slotEmitSearchDone()));
    connect(searchEquForm,SIGNAL(SearchEqument()),this,SLOT(SearchEqument()));
    connect(addEquForm,SIGNAL(sigConnectToSlave(QString)),this,SLOT(slotConnectToSlave(QString)));
    connect(this,SIGNAL(sigConnectedToSlave(bool,paraData*)),addEquForm,SLOT(DisplayResult(bool,paraData*)));
    connect(addEquForm,SIGNAL(sigRegisterSlave(paraData*)),this,SLOT(slotRegisterSlave(paraData *)));
    connect(this,SIGNAL(SearchedEqument(QList<TreeViewItem*>*,QList<paraData*>*)),searchEquForm,SLOT(DisplayResult(QList<TreeViewItem*>*,QList<paraData*>*)));
    connect(searchEquForm,SIGNAL(registerSlaves(QList<paraData*>)),this,SLOT(RegisterSlaves(QList<paraData*>)));
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(slotchangeSlaveDisplay(QModelIndex)));
    connect(ui->treeView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotTreeViewCustomContextMenuRequested(QPoint)));
    connect(tcpServer,SIGNAL(ClientConnect(int,QTcpSocket *)),this,SLOT(ClientConnect(int,QTcpSocket *)));
    connect(this,SIGNAL(sigSendParentTbModel(QSqlTableModel *)),frmSelectParentInTree,SLOT(setTreeViewModel(QSqlTableModel *)));
    //connect(this,SIGNAL(sigSendParentTbModel(QSqlTableModel *)),frmOperateParent,SLOT(setTreeViewModel(QSqlTableModel *)));
    connect(frmSelectParentInTree,SIGNAL(sigSelected(QString)),this,SLOT(slotSelectParentIndex(QString)));
    connect(frmSelectParentInTree,SIGNAL(sigAddNewParentInTree()),this,SLOT(slotAddNewParentInTree()));
    //connect(frmOperateParent,SIGNAL(sigParentInTreeChanged(bool)),this,SLOT(slotParentInTreeDataChanged(bool)));
    connect(treeViewParentTbModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(slotTreeParentChanged(QModelIndex,QModelIndex)));
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(slotchangeSlaveDisplayFromListView(QModelIndex)));
    connect(frmOperateItemBox,SIGNAL(sigoperate(QString)),this,SLOT(sloteditedName(QString)));
    connect(frmSaveSound,SIGNAL(sigoperate(QString)),this,SLOT(sloteditedName(QString)));
    //connect(frmSelectSound,SIGNAL(sigoperate(QString)),this,SLOT(sloteditedName(QString)));
    connect(ui->btnTime,SIGNAL(clicked(bool)),this,SLOT(btnWtiteNowTime()));
    //connect(this,SIGNAL(sigEditFangHuCangPara(paraFangHuCang,quint8)),frmEditSlavePara,SLOT(initPara(paraFangHuCang,quint8)));
    //connect(this,SIGNAL(sigZhaoMingPara(paraZhaoMing,quint8)),frmEditZhaoMing,SLOT(initPara(paraZhaoMing,quint8)));
    connect(ui->btnOpenDoor,SIGNAL(clicked(bool)),this,SLOT(slotOpenDoor()));
    connect(ui->btnReadMsg,SIGNAL(clicked(bool)),this,SLOT(slotShowReadMsgProcess()));
    connect(ui->btnChange0,SIGNAL(clicked(bool)),this,SLOT(slotbtnLed0TextChange()));
    connect(ui->btnChange1,SIGNAL(clicked(bool)),this,SLOT(slotbtnLed1TextChange()));
    connect(ui->btnChange2,SIGNAL(clicked(bool)),this,SLOT(slotbtnLed2TextChange()));
    connect(ui->btnLed0Ok,SIGNAL(clicked(bool)),this,SLOT(slotbtnSetLed0TextOk()));
    connect(ui->btnLed1Ok,SIGNAL(clicked(bool)),this,SLOT(slotbtnSetLed1TextOk()));
    connect(ui->btnLed2Ok,SIGNAL(clicked(bool)),this,SLOT(slotbtnSetLed2TextOk()));
    connect(ui->btnVoiceOk,SIGNAL(clicked(bool)),this,SLOT(slotBtnSetSoundVolume()));
    connect(this,SIGNAL(sigSendVoiceTbModel(QSqlTableModel*)),frmVoiceManager,SLOT(setTreeViewModel(QSqlTableModel*)));
    //connect(this,SIGNAL(sigSendVoiceTbModel(QSqlTableModel*)),frmSelectSound,SLOT(setTreeViewModel(QSqlTableModel*)));

    connect(this,SIGNAL(sigSendModuleTbModel(QList<SlaveVersion *>,QList<paraModule*>)),frmManagerModel,SLOT(slotSetTableModel(QList<SlaveVersion *>,QList<paraModule*>)));
    connect(this,SIGNAL(sigSendModuleTbModel(QSqlTableModel*)),frmUseModule,SLOT(slotSetTableModel(QSqlTableModel*)));

    connect(ui->btnChangeHySound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeHySound()));
    connect(ui->btnChangeSyzSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangesyzSound()));
    connect(ui->btnChangeWhzDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeWhzSound()));
    connect(ui->btnChangeTimeoutWarnSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeTimeoutWarnSound()));
    connect(ui->btnChangeTimeouTiShiSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeTimeoutTiShiSound()));
    connect(ui->btnChangeOpenDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeOpenDoorSound()));
    connect(ui->btnChangeOutDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeOutDoorSound()));
    connect(ui->btnChangeFailLockSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeFailLockSound()));
    connect(ui->btnChangeLaDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnChangeLaDoorSound()));

    connect(frmVoiceManager,SIGNAL(sigDeleteVoice(QString)),this,SLOT(slotDeleteVoice(QString)));
    connect(frmVoiceManager,SIGNAL(sigAddVoicetoDatabase(QString,QString)),this,SLOT(slotAddVoice(QString,QString)));
    connect(this,SIGNAL(sigAddVoiceEnd(int)),frmVoiceManager,SLOT(slotShowAddVoiceEndWindow(int)));

    connect(ui->btnReadHySound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadHySound()));
    connect(ui->btnReadSyzSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadSyzSound()));
    connect(ui->btnReadWhzSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadWhzSound()));
    connect(ui->btnReadTimeouTiShiSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadTimeoutTiShiSound()));
    connect(ui->btnReadTimeoutWarnSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadTimeoutWarnSound()));
    connect(ui->btnReadOutDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadOutDoorSound()));
    connect(ui->btnReadOpenDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadOpenDoorSound()));
    connect(ui->btnReadFailLockSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadFailLockSound()));
    connect(ui->btnReadLaDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnReadLaDoorSound()));
    connect(ui->btnSetFangHuCangPara,SIGNAL(clicked(bool)),this,SLOT(slotShowEditParaDialog()));
    connect(ui->btnSetZhaoMingPrar,SIGNAL(clicked(bool)),this,SLOT(slotShowEditZhaoMingParaDialog()));

    connect(ui->btnPlayHySound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayHySound()));
    connect(ui->btnPlaySyzSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlaySyzSound()));
    connect(ui->btnPlayWhzSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayWhzSound()));
    connect(ui->btnPlayTimeoutWarnSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayTimeoutWarnSound()));
    connect(ui->btnPlayTimeoutTiShiSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayTimeoutTiShiSound()));
    connect(ui->btnPlayOutDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayOutDoorSound()));
    connect(ui->btnPlayOpenDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayOpenDoorSound()));
    connect(ui->btnPlayFailLockSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayFailLockSound()));
    connect(ui->btnPlayLaDoorSound,SIGNAL(clicked(bool)),this,SLOT(slotBtnPlayLaDoorSound()));

    connect(frmVoiceManager,SIGNAL(sigPlaySound(QString)),this,SLOT(slotBtnPlaySound(QString)));

    connect(ui->btnLock,SIGNAL(clicked(bool)),this,SLOT(slotRemoteCloseLock()));
    connect(ui->btnReadMsg,SIGNAL(clicked(bool)),this,SLOT(slotReadSlaveMsg()));
    //connect(frmSelectSound,SIGNAL(sigPlaySound(QString)),this,SLOT(slotBtnPlaySound(QString)));

    //connect(frmManagerModel,SIGNAL(sigAddParaModule()),this,SLOT(slotShowAddModuleWindows()));
    //connect(frmManagerModel,SIGNAL(sigEditParaModule(QModelIndex)),this,SLOT(slotShowEditModuleWindows(QModelIndex)));

    //connect(frmModulePara,SIGNAL(sigEditZhaoMingPara(paraZhaoMing,quint8)),frmEditZhaoMing,SLOT(initPara(paraZhaoMing,quint8)));
    //connect(frmModulePara,SIGNAL(sigShowEditZhaoMingParaWindow()),this,SLOT(slotShowEditZhaoMingParaWindow()));
    //connect(frmEditZhaoMing,SIGNAL(sigSaveZhaoMingPara(paraZhaoMing*)),frmModulePara,SLOT(slotSetZhaoMingPara(paraZhaoMing*)));
    //connect(frmEditZhaoMing,SIGNAL(sigSaveAreaZhaoMingPara(paraZhaoMing*)),this,SLOT(slotSaveAreaSlaveZhaoMingPara(paraZhaoMing*)));

    //connect(frmModulePara,SIGNAL(sigEditFangHuCangPara(paraFangHuCang,quint8)),frmEditSlavePara,SLOT(initPara(paraFangHuCang,quint8)));
    //connect(frmModulePara,SIGNAL(sigShowEditFangHuCangParaWindow()),this,SLOT(slotShowEditFangHuCangParaWindow()));
    //connect(frmEditSlavePara,SIGNAL(sigSaveFangHuCangPara(paraFangHuCang*)),frmModulePara,SLOT(slotSetFangHuCangPara(paraFangHuCang*)));
    //connect(frmEditSlavePara,SIGNAL(sigSaveAreaFangHuCangPara(paraFangHuCang*)),this,SLOT(slotSaveAreaSlaveWorkPara(paraFangHuCang*)));
    //connect(frmModulePara,SIGNAL(sigChangModuleSound(quint8)),this,SLOT(slotChangeModuleSound(quint8)));
    //connect(this,SIGNAL(sigChangeModuleSound(quint8,QString)),frmModulePara,SLOT(slotSetSound(quint8,QString)));

    //connect(frmModulePara,SIGNAL(sigUpdateSoundName(paraData*)),this,SLOT(slotUpdateModuleSoundName(paraData*)));
    //connect(frmModulePara,SIGNAL(sigSaveModulePara(QString,paraData*)),this,SLOT(slotSaveModulePara(QString,paraData*)));
    connect(frmManagerModel,SIGNAL(sigDelParaModule(QModelIndex)),this,SLOT(slotDeleteModule(QModelIndex)));

    connect(frmUseModule,SIGNAL(sigSelectModule(QModelIndex)),this,SLOT(slotUseModule(QModelIndex)));
    connect(frmManagerModel,SIGNAL(sigInParaModule(paraModule*)),this,SLOT(slotSaveModulePara(paraModule*)));
    connect(frmManagerModel,SIGNAL(sigSaveModulePara(paraModule*)),this,SLOT(slotSaveParaModule(paraModule*)));
    connect(frmEditUserPwd,SIGNAL(sigUpdateUserpwd()),this,SLOT(slotUpdateUserPwd()));
    connect(frmEditUserPermission,SIGNAL(sigWriteUserPermitPara(UserPermission*)),this,SLOT(slotEditUserPermission(UserPermission*)));
    //加钞间连接
    connect(ui->btnSetCjcPara,SIGNAL(clicked(bool)),this,SLOT(slotShowSetCjcParaDialog()));
    connect(ui->btnAddKey,SIGNAL(clicked(bool)),this,SLOT(slotbtnAddKey()));
    connect(ui->btnDeleteKey,SIGNAL(clicked(bool)),this,SLOT(slotbtnDeleteKey()));
    connect(ui->btnChangeAdmin,SIGNAL(clicked(bool)),this,SLOT(slotbtnChangeAdminKey()));
    connect(ui->btnEnOrDisAbleFangHuCang,SIGNAL(clicked(bool)),this,SLOT(slotEnableOrDisableSlave()));

    InitStyle();
    //初始化设备和tree结构
    InitForm();
    //显示系统日志记录
    //slotShowSystemLog(systemLogList);

    //获取区域列表
    getParentTreeModel();
    //获取声音列表
    readVoiceData();
    //获取模板列表
    readModuleData(); 
    updateSlaveListDisplay();
    changeStockedPageDisplay(0);
    clearFormStatusDisplay();

    //获取treeview的选择模型 发生变化时触发槽函数
    selectionModel = ui->treeView->selectionModel();
    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
    listViewSelectModel = ui->listView->selectionModel();
    this->setAttribute(Qt::WA_DeleteOnClose,true);//关闭后释放内存
    tcpClient=NULL;
}

frmMain::~frmMain()
{
    delete searchEquForm;
    delete addEquForm;
    delete frmSelectParentInTree;
    delete frmOperateItemBox;
    delete frmSaveSound;
    delete frmVoiceManager;
    delete frmReadSoundProcess;
    //delete frmAddSound;
    delete frmManagerModel;
    delete frmUseModule;
    delete frmAlarm;
    delete frmEditUserPwd;
    delete frmInfo;
    delete frmEditUserPermission;
    delete usingGif;
    delete movie;
    delete connectingGif;
    delete mMediaObj;
    delete ui;
}
void frmMain::ReInitStyle()
{
    if(currentUser.level <= 1)
    {
        editUserPermission->setVisible(true);
    }
    else
    {
        editUserPermission->setVisible(false);
    }
}
void frmMain::InitStyle()
{
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    //设置软件标题
    ui->lab_Title->setText(myApp::SoftTitle);
    location = this->geometry();
    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    //IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

    //菜单栏设计
    menuBar = new QMenuBar(ui->widget);
    menuBar->setGeometry(QRect(0,0,3000,40));

    QMenu* menu = menuBar->addMenu(tr("&设备"));
    QMenu* menu1 = menuBar->addMenu(tr("&参数"));
    QMenu* menuVoice = menuBar->addMenu(tr("&声音"));
    QMenu* menuModule = menuBar->addMenu(tr("&模板"));
    QMenu* menuLog = menuBar->addMenu(tr("&记录"));
    QMenu* menuHelp = menuBar->addMenu(tr("&帮助"));

    actionSearch = menu->addAction(QIcon(":/image/select.png"),tr("&搜索设备"));
    actionSearch->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

    actionAdd = menu->addAction(QIcon(":/image/add.png"),tr("&添加设备"));
    QAction *editUserPwd = menu->addAction(QIcon(":/image/edit.ico"),tr("&修改密码"));

    editUserPermission = menu->addAction(QIcon(":/image/edit.ico"),tr("&修改操作员权限"));

    QAction *editFangHuCangPara = menu1->addAction(QIcon(":/工具栏图标/image/快捷按钮图标/工作参数.png"),tr("工作参数"));
    QAction *editZhaoMingPara = menu1->addAction(QIcon(":/工具栏图标/image/快捷按钮图标/照明参数.png"),tr("照明参数"));
    QAction *managerVoice = menuVoice->addAction(tr("管理声音"));
    QAction *managerModule = menuModule->addAction(tr("管理模板"));
    QAction *useModule = menuModule->addAction(QIcon(":/工具栏图标/image/快捷按钮图标/应用模板.png"),tr("应用模板"));
    //记录菜单下的项
    QAction *loadSlaveLog = menuLog->addAction(QIcon(":/工具栏图标/image/快捷按钮图标/应用模板.png"),tr("读取记录"));
    QAction *searchSlaveLog = menuLog->addAction(QIcon(":/image/search.png"),tr("查询防护舱记录"));
    QAction *searchManualLog = menuLog->addAction(QIcon(":/image/search.png"),tr("查询软件操作记录"));
    //帮助菜单下的项
    QAction *softwareInfo = menuHelp->addAction(QIcon(":/image/info.ico"),tr("关于本软件"));
    QAction *contactUs = menuHelp->addAction(QIcon(":/image/contact us.ico"),tr("软件帮助"));

    ui->gridLayout_2->setMenuBar(menuBar);

    //工具箱建立
    QAction *actionSetClock = new QAction(QIcon(":/使用状态图标/image/使用状态图标/使用状态-使用.gif"),tr("&校准时间"),this);
    toolBar = new QToolBar(this);
    //toolBar->setFixedHeight(55);
    toolBar->addAction(actionSearch);
    toolBar->addAction(actionAdd);
    toolBar->addAction(actionSetClock);
    toolBar->addAction(editFangHuCangPara);
    toolBar->addAction(editZhaoMingPara);
    toolBar->addAction(useModule);
    toolBar->setGeometry(0,0,3000,50);
    //toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//图标在前 文字在后
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//图标在上 文字在下

    ui->gridLayout_3->addWidget(toolBar);

    connect(actionSearch,SIGNAL(triggered()),this,SLOT(searchEqu()));
    connect(actionAdd,SIGNAL(triggered()),this,SLOT(addEqu()));
    connect(editFangHuCangPara,SIGNAL(triggered()),this,SLOT(slotShowEditParaDialog()));
    connect(editZhaoMingPara,SIGNAL(triggered()),this,SLOT(slotShowEditZhaoMingParaDialog()));
    connect(managerVoice,SIGNAL(triggered(bool)),this,SLOT(slotBtnVoiceManager()));
    connect(managerModule,SIGNAL(triggered(bool)),this,SLOT(slotShowManagerModuleWindows()));
    connect(useModule,SIGNAL(triggered(bool)),this,SLOT(slotShowUseModuleWindows()));
    connect(actionSetClock,SIGNAL(triggered(bool)),this,SLOT(btnWtiteNowTime()));
    connect(editUserPwd,SIGNAL(triggered(bool)),this,SLOT(slotShowEditUserPwdWindow()));
    connect(softwareInfo,SIGNAL(triggered(bool)),this,SLOT(slotShowContactWindow()));
    connect(contactUs,SIGNAL(triggered(bool)),this,SLOT(slotOpenHelpFile()));
    connect(editUserPermission,SIGNAL(triggered(bool)),this,SLOT(slotShowEditUserPermissionWindow()));
    connect(loadSlaveLog,SIGNAL(triggered(bool)),this,SLOT(slotReadSlaveMsg()));
    connect(searchSlaveLog,SIGNAL(triggered(bool)),this,SLOT(slotShowSearchSlaveLogWindow()));
    connect(searchManualLog,SIGNAL(triggered(bool)),this,SLOT(slotShowSearchSystemLogWindow()));

    //窗口最大化显示
    on_btnMenu_Max_clicked();
    QPixmap pixmap(myApp::AppPath+myApp::MainPageImage);
    ui->label->setPixmap(pixmap);

    //QStringList header;
    //header<<"参数名称"<<"值";
    //ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->horizontalHeader()->hide();//隐藏表头
    ui->tableWidget->verticalHeader()->hide();//隐藏行号
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,118);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("设备名称"));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("设备类型"));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("版本号"));
    ui->tableWidget->setItem(3,0,new QTableWidgetItem("MAC地址"));
    ui->tableWidget->setItem(4,0,new QTableWidgetItem("IP地址"));
    ui->tableWidget->setItem(5,0,new QTableWidgetItem("联网模式"));

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
    /**********************加钞间参数标签页**********************/

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
    //系统日志显示窗口初始化
    ui->systemLog->setColumnCount(2);
    ui->systemLog->horizontalHeader()->hide();//隐藏表头
    ui->systemLog->verticalHeader()->hide();//隐藏行号
    ui->systemLog->horizontalHeader()->setStretchLastSection(true);//关键

    ui->systemLog->setColumnWidth(0, 150);
    ui->systemLog->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->systemLog->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //防护舱日志显示窗口初始化
    ui->tableWidgetSlaveLog->setColumnCount(3);
    ui->tableWidgetSlaveLog->horizontalHeader()->hide();//隐藏表头
    ui->tableWidgetSlaveLog->verticalHeader()->hide();//隐藏行号
    ui->tableWidgetSlaveLog->horizontalHeader()->setStretchLastSection(true);//关键

    ui->tableWidgetSlaveLog->setColumnWidth(0, 150);
    ui->tableWidgetSlaveLog->setColumnWidth(1, 200);
    ui->tableWidgetSlaveLog->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidgetSlaveLog->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->tableWidgetAlarmPara->setColumnCount(2);
    ui->tableWidgetAlarmPara->setRowCount(9);
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
    ui->tableWidgetAlarmPara->setItem(7,0,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(8,0,new QTableWidgetItem(""));
    for(int i=0;i<9;i++)
    {
        ui->tableWidgetAlarmPara->item(i,0)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    }

    //窗口分隔比例设置
    if(myHelper::FileIsExist("AppConfig.ini"))
    {
        readSettings();
    }
    else
    {
        ui->splitter_2->setStretchFactor(0,8);//树型结构占用空间比例
        ui->splitter_2->setStretchFactor(1,12);//其他占用空间比例

        ui->splitter->setStretchFactor(0,5);//其他占用空间比例
        ui->splitter->setStretchFactor(1,8);//日志输出占用空间比例

        ui->splitter_3->setStretchFactor(0,12);//设备列表占用空间比例
        ui->splitter_3->setStretchFactor(1,12);//设备参数占用空间比例
    }

}

void frmMain::InitForm()
{
    bool exitInList=false;

    //读取分支树父辈
    if(!query->exec(select_Parent_sql))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {

            TreeViewItem *treeItem = new TreeViewItem();
            treeItem->text = query->value(0).toString();
            treeItem->index = query->value(1).toInt();
            treeItem->mac = treeItem->index;
            treeItem->setSlave(NULL);
            treeItem->parentIndexInTree = -1;
            for(int i=0;i<viewlist.count();i++)
            {
                if(viewlist.at(i)->text == treeItem->text)
                {
                    exitInList = true;
                    break;
                }
            }
            if(!exitInList)
            {

                viewlist.append(treeItem);
            }
            exitInList=false;
        }
    }

    //读取设备
    if(!query->exec(select_TSlave_sql))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            //构建参数类实例
            paraData *pdat = new paraData();
            bool autolink;

            TreeViewItem *treeItem = new TreeViewItem();
            treeItem->mac=query->value(0).toString();
            pdat->net.macAddr = treeItem->mac;

            for(int i=0;i<viewlist.count();i++)
            {
                if(viewlist.at(i)->mac == treeItem->mac)
                {
                    exitInList = true;
                    break;
                }
            }
            if(exitInList)
            {
                continue;
                exitInList=false;
            }
            pdat->slaveType = query->value(1).toInt();

            pdat->version = query->value(2).toString();

            QStringList verlist = pdat->version.split(".");
            pdat->mainVer = verlist.at(0).toInt();
            pdat->secondVer = verlist.at(1).toInt();
            pdat->waiSheModel = verlist.at(2).toInt();
            pdat->xiuDingVer = verlist.at(3).toInt();

            treeItem->text = query->value(3).toString();
            pdat->name = treeItem->text;
            QString tempstr="";
            //添加日志记录
            switch (pdat->slaveType) {
            case 0:
                tempstr="加载防护舱“";
                break;
            case 1:
                tempstr="加载加钞间“";
                break;
            default:
                break;
            }
            InsertOneSystemLogAndShow(tempstr+pdat->name+"("+pdat->net.macAddr+")"+"”");

            pdat->net.ip = query->value(4).toString();

            autolink = query->value(5).toBool();

            treeItem->index = query->value(6).toInt();
            //读取设备的网络参数
            QSqlQuery *selectSlaveNetWork = new QSqlQuery(database);
            selectSlaveNetWork->prepare(select_TNetWork_sql);
            selectSlaveNetWork->bindValue(":mac",pdat->net.macAddr);
            if(!selectSlaveNetWork->exec())
            {
                QLOG_ERROR() <<selectSlaveNetWork->lastError();
            }
            else
            {
                while(selectSlaveNetWork->next())
                {
                    pdat->net.macAddr = selectSlaveNetWork->value(0).toString();
                    pdat->net.ip = selectSlaveNetWork->value(1).toString();
                    pdat->net.gateWay = selectSlaveNetWork->value(2).toString();
                    pdat->net.subnetMask = selectSlaveNetWork->value(3).toString();
                    pdat->net.networkModel = selectSlaveNetWork->value(4).toInt();
                    pdat->net.remoteIp = selectSlaveNetWork->value(5).toString();
                }
            }

            QSqlQuery *selectParaentId = new QSqlQuery(database);
            //查找mac地址对应的父的名称
            QString parentname="";//父的名称
            //int parentid=0;
            QString strSql = select_macParent_sql+"'"+treeItem->mac+"'";
            selectParaentId->prepare(strSql);
            if(!selectParaentId->exec())
            {
                QLOG_ERROR() <<selectParaentId->lastError();
            }
            else
            {
                while(selectParaentId->next())
                {
                    parentname = selectParaentId->value(0).toString();
                }
            }
            //读取对应父的indexInTree
            strSql = select_indexParent_sql+"'"+parentname+"'";
            selectParaentId->prepare(strSql);
            if(!selectParaentId->exec())
            {
                QLOG_ERROR() <<selectParaentId->lastError();
            }
            else
            {
                while(selectParaentId->next())
                {
                    treeItem->parentIndexInTree = selectParaentId->value(0).toInt();
                    //qDebug()<<treeItem->parentId;
                }
            }
            delete selectParaentId;
            QSqlQuery *readPara = new QSqlQuery(database);

            //读取参数  根据设备型号pdat->slaveType判断需要读取的参数
            /************************************************************/

            readPara->prepare(select_TParamater_sql);
            readPara->bindValue(":mac",pdat->net.macAddr);
            if(!readPara->exec())
            {
                QLOG_ERROR() <<readPara->lastError();
            }
            else
            {
                while(readPara->next())
                {
                    pdat->fangHuCang.openLockTime     = readPara->value(1).toInt();
                    pdat->fangHuCang.OptBussinessTime = readPara->value(2).toInt();
                    pdat->fangHuCang.timeOutRemind    = readPara->value(3).toInt();
                    pdat->fangHuCang.warnningDelayTime= readPara->value(4).toInt();
                    pdat->fangHuCang.noManOpenLockTime= readPara->value(5).toInt();
                    pdat->fangHuCang.fanRunTemperature= readPara->value(6).toInt();
                    pdat->fangHuCang.signalModel      = readPara->value(7).toInt();
                    pdat->fangHuCang.peopleEquModel   = readPara->value(8).toInt();
                    pdat->fangHuCang.lockModel        = readPara->value(9).toInt();
                    pdat->fangHuCang.kongCangLockorNot= readPara->value(10).toInt();
                    pdat->fangHuCang.fangQiewarnning  = readPara->value(11).toInt();

                    pdat->zhaoMing.Model      =readPara->value(12).toInt();
                    pdat->zhaoMing.startHour  = readPara->value(13).toInt();
                    pdat->zhaoMing.startMinute= readPara->value(14).toInt();
                    pdat->zhaoMing.endHour    = readPara->value(15).toInt();
                    pdat->zhaoMing.endMinute  = readPara->value(16).toInt();
                    pdat->zhaoMing.manEnable = readPara->value(17).toInt();

                    pdat->ledText[0]= readPara->value(18).toString();
                    pdat->ledText[1]= readPara->value(19).toString();
                    pdat->ledText[2]= readPara->value(20).toString();

                    pdat->soundVolume=readPara->value(21).toInt();
                    pdat->soundMd5[0]=myHelper::hexStringtoByteArray(readPara->value(22).toString());

                    pdat->soundMd5[2]=myHelper::hexStringtoByteArray(readPara->value(23).toString());
                    pdat->soundMd5[3]=myHelper::hexStringtoByteArray(readPara->value(24).toString());
                    pdat->soundMd5[4]=myHelper::hexStringtoByteArray(readPara->value(25).toString());
                    pdat->soundMd5[5]=myHelper::hexStringtoByteArray(readPara->value(26).toString());
                    pdat->soundMd5[6]=myHelper::hexStringtoByteArray(readPara->value(27).toString());
                    pdat->soundMd5[7]=myHelper::hexStringtoByteArray(readPara->value(28).toString());
                    pdat->soundMd5[1]=myHelper::hexStringtoByteArray(readPara->value(29).toString());
                    pdat->soundMd5[8]=myHelper::hexStringtoByteArray(readPara->value(30).toString());
                    //读取报警设置参数
                    pdat->alarmPara.btnAlarmEnable = readPara->value(31).toBool();
                    pdat->alarmPara.cutAlarmEnable = readPara->value(32).toBool();
                    pdat->alarmPara.zhengDongAlarmEnable = readPara->value(33).toBool();
                    pdat->alarmPara.yanWuAlarmEnable = readPara->value(34).toBool();
                    pdat->alarmPara.boLiAlarmEnable = readPara->value(35).toBool();
                    pdat->alarmPara.shuiQinAlarmEnable = readPara->value(36).toBool();
                    pdat->alarmPara.tempAlarmEnable = readPara->value(37).toBool();
                    /***************************/
                    //加钞间独有的参数
                    pdat->fangHuCang.userNum=readPara->value(38).toInt();;             //D2 刷卡人数（1-10人）N+1，如果设置为0表示单卡进出，不为0表示多卡
                    pdat->fangHuCang.systemAlarmStatus=readPara->value(39).toInt();;   //D3 设置系统报警状态：0 - 撤防；1 - 设防
                    pdat->fangHuCang.setGuardDelayTime=readPara->value(40).toInt();;   //D4 设防延时时间（1-90秒钟）
                    pdat->fangHuCang.isMonitorOrNot=readPara->value(41).toInt();;      //D5 是否有人值守：0 - 无人值守；1 - 有人值守
                    pdat->fangHuCang.inDoorModel=readPara->value(42).toInt();;         //D6: 进门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码
                    pdat->fangHuCang.outDoorModel=readPara->value(43).toInt();;
                    //报警参数
                    pdat->alarmPara.doorCiAlarmEnable=readPara->value(44).toBool();;      //非法开门报警
                    pdat->alarmPara.existManAlarmEnable=readPara->value(45).toBool();;  //非法入侵报警
                }
            }
            delete readPara;
            //构建设备类实例
            Equipment *temp=new Equipment(this);
            temp->autoLink = autolink;
            connectToUpdateSlaveToDB(temp);
            temp->EquInit(*pdat);

            //将树结构和设备类关联在一起
            treeItem->setSlave(temp);
            //更新树结构
            viewlist.append(treeItem);
            //获取声音对应的名字
            getSlaveSoundName(temp->GetEquAll());
        }
    }
    //读取模板类
    if(!query->exec("select * from TMparamater"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {

            paraModule * module = new paraModule();
            module->name = query->value(0).toString();
            module->pdat.fangHuCang.openLockTime     = query->value(1).toInt();
            module->pdat.fangHuCang.OptBussinessTime = query->value(2).toInt();
            module->pdat.fangHuCang.timeOutRemind    = query->value(3).toInt();
            module->pdat.fangHuCang.warnningDelayTime= query->value(4).toInt();
            module->pdat.fangHuCang.noManOpenLockTime= query->value(5).toInt();
            module->pdat.fangHuCang.fanRunTemperature= query->value(6).toInt();
            module->pdat.fangHuCang.signalModel      = query->value(7).toInt();
            module->pdat.fangHuCang.peopleEquModel   = query->value(8).toInt();
            module->pdat.fangHuCang.lockModel        = query->value(9).toInt();
            module->pdat.fangHuCang.kongCangLockorNot= query->value(10).toInt();
            module->pdat.fangHuCang.fangQiewarnning  = query->value(11).toInt();

            module->pdat.zhaoMing.Model      =query->value(12).toInt();
            module->pdat.zhaoMing.startHour  = query->value(13).toInt();
            module->pdat.zhaoMing.startMinute= query->value(14).toInt();
            module->pdat.zhaoMing.endHour    = query->value(15).toInt();
            module->pdat.zhaoMing.endMinute  = query->value(16).toInt();

            module->pdat.ledText[0]= query->value(18).toString();
            module->pdat.ledText[1]= query->value(19).toString();
            module->pdat.ledText[2]= query->value(20).toString();

            module->pdat.soundVolume=query->value(21).toInt();
            module->pdat.soundMd5[0]=myHelper::hexStringtoByteArray(query->value(22).toString());
            module->pdat.soundMd5[2]=myHelper::hexStringtoByteArray(query->value(23).toString());
            module->pdat.soundMd5[3]=myHelper::hexStringtoByteArray(query->value(24).toString());
            module->pdat.soundMd5[4]=myHelper::hexStringtoByteArray(query->value(25).toString());
            module->pdat.soundMd5[5]=myHelper::hexStringtoByteArray(query->value(26).toString());
            module->pdat.soundMd5[6]=myHelper::hexStringtoByteArray(query->value(27).toString());
            module->pdat.soundMd5[7]=myHelper::hexStringtoByteArray(query->value(28).toString());
            module->pdat.soundMd5[1]=myHelper::hexStringtoByteArray(query->value(29).toString());
            module->pdat.soundMd5[8]=myHelper::hexStringtoByteArray(query->value(30).toString());
            //读取报警设置参数
            module->pdat.alarmPara.btnAlarmEnable = query->value(31).toBool();
            module->pdat.alarmPara.cutAlarmEnable = query->value(32).toBool();
            module->pdat.alarmPara.zhengDongAlarmEnable = query->value(33).toBool();
            module->pdat.alarmPara.yanWuAlarmEnable = query->value(34).toBool();
            module->pdat.alarmPara.boLiAlarmEnable = query->value(35).toBool();
            module->pdat.alarmPara.shuiQinAlarmEnable = query->value(36).toBool();
            module->pdat.alarmPara.tempAlarmEnable = query->value(37).toBool();
            /***************************/
            //加钞间独有的参数
            module->pdat.fangHuCang.userNum = query->value(38).toInt();;             //D2 刷卡人数（1-10人）N+1，如果设置为0表示单卡进出，不为0表示多卡
            module->pdat.fangHuCang.systemAlarmStatus = query->value(39).toInt();;   //D3 设置系统报警状态：0 - 撤防；1 - 设防
            module->pdat.fangHuCang.setGuardDelayTime = query->value(40).toInt();;   //D4 设防延时时间（1-90秒钟）
            module->pdat.fangHuCang.isMonitorOrNot = query->value(41).toInt();;      //D5 是否有人值守：0 - 无人值守；1 - 有人值守
            module->pdat.fangHuCang.inDoorModel = query->value(42).toInt();;         //D6: 进门方式 0-指纹；1-ID卡；2-TM卡；3-ID卡+密码；4-密码
            module->pdat.fangHuCang.outDoorModel = query->value(43).toInt();;
            //报警参数
            module->pdat.alarmPara.doorCiAlarmEnable = query->value(44).toBool();;      //非法开门报警
            module->pdat.alarmPara.existManAlarmEnable = query->value(45).toBool();;  //非法入侵报警
            moduleList.append(module);
        }
    }
    for(int j=0;j<moduleList.count();j++)
    {
        paraData * para = &moduleList.at(j)->pdat;
        for(int i=0;i<9;i++)
        {
            QString fileName="";
            QString name="";
            QString md5 = para->soundMd5[i].toHex();
            Select_TVoice_FileName_Name(name,fileName,md5);
            para->voiceFileName[i]=fileName;
            para->soundName[i]= name;
        }
    }

    //读取操作员账户权限
    if(!query->exec("select * from TPermit"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        if(query->next())
        {

            guestPermission.level                   = query->value(0).toInt();
            guestPermission.permitEditSystemPar     = query->value(1).toBool();
            guestPermission.permitEditAreaInfo      = query->value(2).toBool();
            guestPermission.permitRegFangHuCang     = query->value(3).toBool();
            guestPermission.permitLogoutFangHuCang  = query->value(4).toBool();
            guestPermission.permitEditSoundLiabrary = query->value(5).toBool();
            guestPermission.permitEditModule        = query->value(6).toBool();

            guestPermission.permitEditFangHuCangPar     = query->value(7).toBool();
            guestPermission.permitEditZhaoMingPar       = query->value(8).toBool();
            guestPermission.permitEditVoicePar          = query->value(9).toBool();
            guestPermission.permitAdjustTime            = query->value(10).toBool();
            guestPermission.permitOpenDoor              = query->value(11).toBool();
            guestPermission.permitUseorNotusedFangHuCang= query->value(12).toBool();
            guestPermission.permitLockorOpenFangHuCang  = query->value(13).toBool();
            guestPermission.permitEditLedText           = query->value(14).toBool();
            guestPermission.permitChangeAdmin           = query->value(15).toBool();

        }
    }
    //读取版本号和名称
    versionList.clear();
    if(!query->exec("select tmodule.[Name],TMODULE.[level],TVERSIONMANAGER.[Name] as version from TMODULE join TVERSIONMANAGER ON TMODULE.[level] = TVERSIONMANAGER.[Level]"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            SlaveVersion *tempVersion = new SlaveVersion();

            tempVersion->name     = query->value(0).toString();
            tempVersion->level    = query->value(1).toInt();
            tempVersion->version = query->value(2).toString();

            versionList.append(tempVersion);
        }
    }
    //读取版本号和等级
    if(!query->exec("select * from TVersionManager"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            VersionInfo *tempVersion = new VersionInfo();

            tempVersion->level    = query->value(0).toInt();
            tempVersion->name     = query->value(1).toString();

            versioninfoList.append(tempVersion);
        }
    }

    for(int i=0;i<moduleList.length();i++)
    {
        QString moduleName = moduleList.at(i)->name;
        if(!query->exec("select level from TModule where Name='"+moduleName+"'"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                moduleList.at(i)->level = query->value(0).toInt();
            }
        }
    }
    /*
    if(!query->exec("select * from TSystemLog"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            qDebug()<<query->value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            qDebug()<<query->value(1).toString();
        }

    }
    query->prepare("insert into TSystemLog values (?, ?)");
    //父名称
    QString str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //QString str = QDateTime::currentDateTime().toString("s");
    query->addBindValue(str);
    //在树中的index
    query->addBindValue("测试");

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
    */
}
//关联每个设备都要关联的信号
void frmMain::connectToUpdateSlaveToDB(Equipment *slave)
{
    connect(slave,SIGNAL(sigDataUpdateToDataBase(Equipment*)),this,SLOT(slotUpdateDatabase(Equipment*)));
    connect(slave,SIGNAL(sigUpdateTSlave(Equipment*)),this,SLOT(slotUpdateTSlave(Equipment*)));
    connect(slave,SIGNAL(sigSlaveWarm(Equipment*)),this,SLOT(slotShowWarningWindowAndPlaySound(Equipment*)));
}
//解除关联每个设备都要关联的信号
void frmMain::disconnectToUpdateSlaveToDB(Equipment *slave)
{
    disconnect(slave,SIGNAL(sigDataUpdateToDataBase(Equipment*)),this,SLOT(slotUpdateDatabase(Equipment*)));
    disconnect(slave,SIGNAL(sigUpdateTSlave(Equipment*)),this,SLOT(slotUpdateTSlave(Equipment*)));
    disconnect(slave,SIGNAL(sigSlaveWarm(Equipment*)),this,SLOT(slotShowWarningWindowAndPlaySound(Equipment*)));
}
bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void frmMain::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void frmMain::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void frmMain::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void frmMain::on_btnMenu_Close_clicked()
{
    /*
    //将日志记录写入数据库
    QLOG_INFO()<<"将日志记录写入数据库...";
    for(int i=0;i<fangHuCangLogList.count();i++)
    {
        query->prepare("insert into TFangHuCangLog values (?,?,?)");
        //日期时间
        //QString str = fangHuCangLogList.at(i)->dateTime.toString("yyyy-MM-dd hh:mm:ss");
        query->addBindValue(fangHuCangLogList.at(i)->dateTime);
        //防护舱名称
        query->addBindValue(fangHuCangLogList.at(i)->name);
        //日志内容
        query->addBindValue(fangHuCangLogList.at(i)->logItem);

        if(!query->exec())
        {
            QLOG_ERROR() <<query->lastError();
        }
    }
    QLOG_INFO()<<"将软件操作记录写入数据库...";
    for(int i=0;i<systemLogList.count();i++)
    {
        query->prepare("insert into TSystemLog values (?,?)");
        //日期时间
        query->addBindValue(systemLogList.at(i)->dataTime);
        //日志内容
        query->addBindValue(systemLogList.at(i)->LogItem);

        if(!query->exec())
        {
            QLOG_ERROR() <<query->lastError();
        }
    }*/

    //保存配置
    QLOG_INFO()<<"保存配置...";
    writeSettings();
    //关闭数据库
    QLOG_INFO()<<"关闭数据库...";
    database.close();
    //qApp->exit();
    QLOG_INFO()<<"关闭窗口，退出程序。";
    this->close();
}

void frmMain::on_btnMenu_Max_clicked()
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

void frmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void frmMain::searchEqu()
{
    searchedAll.clear();
    searchEquForm->setModal(true);
    searchEquForm->DisplayResult(&viewlist,&searchedAll);
    //emit SearchedEqument(&viewlist,&searchedAll);
    searchEquForm->show();
    //searchEquForm->exec();
}
void frmMain::addEqu()
{
    addEquForm->setModal(true);
    addEquForm->show();
    //addEquForm->exec();
}
void frmMain::readPacketage()
{

    QByteArray datagram; //拥于存放接收的数据报
    QByteArray check;
    QHostAddress addr;
    quint8 rcvCount=0;
      //拥有等待的数据报
    do
    {
        datagram.resize(udpClient->pendingDatagramSize());
        udpClient->readDatagram(datagram.data(),datagram.size(),&addr);
        rcvCount+=udpClient->pendingDatagramSize();
        //写入缓冲区
        comBuffer->write(datagram.data(),datagram.length());
        QLOG_INFO() << "receive UDP data：" << datagram.toHex();
        searchTimeOut->start(2000);
    }while(udpClient->hasPendingDatagrams());


    //发送信号处理报文
    //emit sigAnalysisData();
}
//分析接收的数据
void frmMain::slotAnalysisData()
{

    static quint8 startBuf=0;
    static quint8 func=0;
    static char dbuf[500];//读取控制器记录最多230个字节
    static quint8 ret=1;
    static quint16 blockSize=0;
    while(!comBuffer->isEmpty())
    {
        if (!comBuffer->isEmpty())
        {
            //处理完一帧数据后func置0 重新读取功能码
            if(func == 0)
            {
                comBuffer->read((char*)&dbuf[0], 1);
                QLOG_INFO()<<"dbuf[0]:"<<(quint8)dbuf[0];
                if((quint8)dbuf[0] < KEY)
                {
                    comBuffer->read((char*)&dbuf[0], 3);
                    continue;
                }
                func=((quint8)dbuf[0]-KEY);

                QLOG_INFO()<<"func:"<<func;

                //计算需要读取的数据的个数
                if(func == 1)
                {
                    blockSize = 71;
                    startBuf = 1;
                }
                else if(func ==3)
                {
                    blockSize = 71;
                    startBuf = 1;
                }
                else
                {
                    comBuffer->setEmpty();
                    break;
                }
            }
            //QLOG_INFO()<<"blockSize:"<<blockSize;
            //QLOG_INFO()<<"startBuf:"<<startBuf;
            if(comBuffer->getFreeSize()>=blockSize)
            {

                //读取剩余数据并处理
                comBuffer->read((char*)&dbuf[startBuf],blockSize);

                paraData *pdata=new paraData();
                if(!Func_Udp_01((char*)&dbuf,pdata))
                {
                    if(func == 1)
                    {
                        searchedAll.append(pdata);
                        //设置超时时间为1秒 如果超过1秒则认为搜索完成
                    }
                    else if(func == 3)
                    {
                        bool registered=false;
                        //使用输入的ip 因为可能是远程ip
                        pdata->net.ip = addSlaveIp;
                        for(int i=0;i<viewlist.count();i++)
                        {
                            if(viewlist.at(i)->mac == pdata->net.macAddr)
                            {
                                registered = true;
                                break;
                            }
                        }
                        emit sigConnectedToSlave(registered,pdata);
                    }
                    else
                    {
                        delete pdata;
                    }
                }
                func=0;//表示数据处理一次完毕
            }
            else
            {
                //接收数据不完整
                break;
            }

        }
        /*
        else//如果没有这个等待，就会有很多空循环，CPU会很高。
        {
            comBuffer->waitNotEmpty();
        }*/
    }
    emit SearchedEqument(&viewlist,&searchedAll);

}

void frmMain::sendUdpPacketage(QHostAddress address,quint8 func,quint16 port)
{
    udpClient->bind(myApp::UdpPort.toInt());
    udpClient->open(QIODevice::ReadWrite);
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    uchar header=func;
    quint8 dataCount=0;
    uchar D[100]={0};
    uchar crc=0;
    switch(func)
    {
    case 1:
        dataCount=2;
        D[0]=0;
        D[1]=0;
    break;
    case 3:
        dataCount=2;
        D[0]=0;
        D[1]=0;
    break;
    default:
        return;
    break;
    }
    //写入包头
    out<<header;
    //写入数据
    for(int i=0;i<dataCount;i++)
    {
        out<<D[i];
    }
    crc = getCRC((uchar *)datagram.data(),dataCount+1);

    out<<crc;

    udpClient->writeDatagram(datagram,address,port);

    searchTimeOut->start(5000);
}
void frmMain::slotConnectToSlave(QString ip)
{
    if(myHelper::IsIP(ip))
    {
        //connect(udpClient,SIGNAL(readyRead()),this,SLOT(readPacketage()));
        addSlaveIp=ip;
        QHostAddress address=QHostAddress(ip);
        sendUdpPacketage(address,3,3611);
    }
}
void frmMain::SearchEqument()
{
    //connect(udpClient,SIGNAL(readyRead()),this,SLOT(readPacketage()));
    QLOG_INFO()<<"search slave……";
    QHostAddress address=QHostAddress::Broadcast;
    searchedAll.clear();

    sendUdpPacketage(address,1,3611);
}
void frmMain::CreatNewSlave(paraData prData)
{

}
void frmMain::RegisterSlaves(QList<paraData*> registerList)
{
    if(!checkPermission(guestPermission.permitRegFangHuCang))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许注册防护舱！");
        return;
    }
    if(!(registerList.count()>0)) return;                    //如果没有搜索到的设备则返回
    selectedParentName="";                                  //清除上次选择的区域的名称
    emit sigSendParentTbModel(treeViewParentTbModel);       //发送区域信号
    frmSelectParentInTree->exec();                          //运行选择窗口
    if(frmSelectParentInTree->result()==QDialog::Rejected)
    {
        return;//如果点击取消或关闭则返回
    }
    if(selectedParentName!="")
    {
        //如果选择的区域的名称为空则不执行
        //查找选择的区域的index
        int parentIndex=0;
        int childs=0;
        if(!query->exec("SELECT IndexInTree FROM TZone WHERE ZName='"+selectedParentName+"'"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                parentIndex = query->value(0).toInt();
                //qDebug()<<"parentIndex"<<parentIndex;
            }
        }
        //获取所选区域的child的个数
        if(!query->exec("SELECT Mac FROM TBelong WHERE ZName='"+selectedParentName+"'"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                childs++;
            }
        }

        searchedNoRegister.clear();
        searchedNoRegister = removeRegisteredSlave(registerList);
        for(int i=0;i<searchedNoRegister.count();i++)
        {
            //判断是否达到了最大连接数量
            if(viewlist.count() >= MaxConnectSlave)
            {
                myHelper::ShowMessageBoxError("连接数量已经达到限制！无法添加新防护舱！");
                return;
            }
            Equipment *temp=new Equipment(this);
            temp->parentIndexInTree=parentIndex;    //所属区域的index
            temp->parentName = selectedParentName;//所属区域的名称
            temp->indexInTree = childs++;
            if(isremote)
            {
                temp->setEquQTcpSocket(tcpClient);
                isremote=false;
            }
            temp->EquInit(*searchedAll.at(i));
            //判断版本是否支持
            QStringList versionlist = ConnectVersion.split(",");
            if(temp->equParaData.slaveType == 0)
            {
                switch(temp->equParaData.getTwoMainVer())
                {
                case 27:
                    break;
                case 28:
                    break;
                default:
                    myHelper::ShowMessageBoxError("版本不兼容！无法添加新防护舱！");
                    return;
                    break;
                }
            }
            //classList.append(temp);
            //窗口更新显示
            TreeViewItem *treeItem = new TreeViewItem();
            treeItem->text = temp->equParaData.name;
            treeItem->index = temp->indexInTree;
            treeItem->parentIndexInTree = temp->parentIndexInTree;
            treeItem->mac = temp->equParaData.net.macAddr;
            //关联信号
            connectToUpdateSlaveToDB(temp);
            treeItem->setSlave(temp);

            viewlist.append(treeItem);

            updateSlaveListDisplay();
            //写入数据库
            InsertEquipment(temp);
        }
    }

}
bool frmMain::checkPermission(bool permit)
{
    if(currentUser.level ==2)
    {
        if(!permit)
        {
            return false;
        }
    }
    return true;
}
void frmMain::slotRegisterSlave(paraData *registered)
{

    if(!checkPermission(guestPermission.permitRegFangHuCang))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许注册防护舱！");
        return;
    }

    if(!registered) return;                    //如果没有搜索到的设备则返回
    selectedParentName="";                                 //清除上次选择的区域的名称
    emit sigSendParentTbModel(treeViewParentTbModel);       //发送区域信号
    frmSelectParentInTree->exec();                          //运行选择窗口
    if(frmSelectParentInTree->result()==QDialog::Rejected)
    {
        return;//如果点击取消或关闭则返回
    }
    if(selectedParentName!="")
    {
        //如果选择的区域的名称为空则不执行
        //查找选择的区域的index
        int parentIndex=0;
        int childs=0;
        if(!query->exec("SELECT IndexInTree FROM TZone WHERE ZName='"+selectedParentName+"'"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                parentIndex = query->value(0).toInt();
                //qDebug()<<"parentIndex"<<parentIndex;
            }
        }
        //获取所选区域的child的个数
        if(!query->exec("SELECT Mac FROM TBelong WHERE ZName='"+selectedParentName+"'"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                childs++;
            }
        }

            Equipment *temp=new Equipment(this);
            temp->parentIndexInTree=parentIndex;    //所属区域的index
            temp->parentName = selectedParentName;//所属区域的名称
            temp->indexInTree = childs++;
            temp->EquInit(*registered);
            //窗口更新显示
            TreeViewItem *treeItem = new TreeViewItem();
            treeItem->text = temp->equParaData.name;
            treeItem->index = temp->indexInTree;
            treeItem->parentIndexInTree = temp->parentIndexInTree;
            treeItem->mac = temp->equParaData.net.macAddr;
            //关联信号
            connectToUpdateSlaveToDB(temp);
            treeItem->setSlave(temp);

            viewlist.append(treeItem);

            updateSlaveListDisplay();
            //写入数据库
            InsertEquipment(temp);

    }

}
void frmMain::updateSlaveListDisplay()
{
    viewlist = getSortedList(viewlist);
    model->setData(viewlist);
    ui->treeView->setModel(model);
    ui->treeView->expandAll(); //结点全部展开
}
//更换主页显示
void frmMain::changeStockedPageDisplay(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
//
void frmMain::slotShowEditParaDialog()
{
    if(!checkPermission(guestPermission.permitEditFangHuCangPar))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许修改工作参数！");
        return;
    }
    if(currentSlave)
    {
       if(currentSlave->GetEquAll()->slaveStatus.connectStatus == 1)
       {
           frmEditParamater *frmEditSlavePara = new frmEditParamater();
           frmEditSlavePara->initPara(currentSlave->GetEquAll()->fangHuCang,0);
           //emit sigEditFangHuCangPara(currentSlave->GetEquAll()->fangHuCang,0);
           connect(frmEditSlavePara,SIGNAL(sigWriteFangHuCangPara(paraFangHuCang*)),currentSlave,SLOT(slotSetWorkPara(paraFangHuCang*)));
           frmEditSlavePara->setModal(true);
           frmEditSlavePara->show();
           //delete frmEditSlavePara;
       }
       else
       {
           myHelper::ShowMessageBoxInfo("设备未连接！");
       }
    }
    else
    {
        myHelper::ShowMessageBoxInfo("请首先选择设备！");
    }
}
void frmMain::slotShowEditZhaoMingParaDialog()
{
    if(!checkPermission(guestPermission.permitEditZhaoMingPar))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许修改照明参数！");
        return;
    }
    if(currentSlave)
    {
        if(currentSlave->GetEquAll()->slaveStatus.connectStatus == 1)
        {
            frmEditZhaoMingPara *frmEditZhaoMing = new frmEditZhaoMingPara();
            frmEditZhaoMing->initPara(currentSlave->GetEquAll()->zhaoMing,0);
            //emit sigZhaoMingPara(currentSlave->GetEquAll()->zhaoMing,0);
            connect(frmEditZhaoMing,SIGNAL(sigWriteZhaoMingPara(paraZhaoMing*)),currentSlave,SLOT(slotSetLineAndAdLinePara(paraZhaoMing*)));
            frmEditZhaoMing->setModal(true);
            frmEditZhaoMing->show();
        }
        else
        {
            myHelper::ShowMessageBoxInfo("设备未连接！");
        }
    }
    else
    {
        myHelper::ShowMessageBoxInfo("请首先选择设备！");
    }
}
void frmMain::slotShowEditZhaoMingParaWindow()
{
  //frmEditZhaoMing->exec();
}
void frmMain::slotShowEditFangHuCangParaWindow()
{
    //frmEditSlavePara->exec();
}
//刷新时间
void frmMain::slotupdateSlaveTimeDisplay(paraData* pdata)
{
    if(pdata)
    {
        //时间显示
        slaveTime = pdata->slaveDate.toString("yy年MM月d日")+" "+pdata->slaveTime.toString("hh时mm分ss秒");
        ui->leSlaveDateTime->setText(slaveTime);
    }
}
//状态页面显示项目修改为防护舱界面
void frmMain::changeFHCText()
{
    if(windowStatus!=1)
    {
        windowStatus=1;
        //设置防护舱参数标签可见
        //disconnect(ui->btnEnOrDisAbleFangHuCang,SIGNAL(clicked(bool)),this,SLOT(btnEnOrDisAbleFangHuCang()));
        //connect(ui->btnEnOrDisAbleFangHuCang,SIGNAL(clicked(bool)),this,SLOT(slotEanbleOrDisableSlave()));
        ui->tabWidgetSlavePara->clear();
        ui->tabWidgetSlavePara->addTab(ui->tabSlaveStatus,"运行状态");
        ui->tabWidgetSlavePara->addTab(ui->tabSlavePara,"控制参数");
        ui->tabWidgetSlavePara->addTab(ui->tabVoicePara,"语音状态");
        ui->tabWidgetSlavePara->addTab(ui->tabLedPara,"LED参数");
        ui->tabWidgetSlavePara->addTab(ui->tabAlarmPara,"报警参数");
        //ui->tabWidgetSlavePara->setCurrentIndex(0);
        ui->btnOpenDoor->setEnabled(true);
        ui->btnOpenDoor->setStyleSheet("");
        ui->btnOpenDoor->setText("开门");

        ui->statusLabel_1->setText("使用状态：");
        ui->statusLabel_2->setText("门状态：");
        ui->statusLabel_3->setText("照明灯：");
        ui->statusLabel_4->setText("防护舱状态：");
        ui->statusLabel_5->setText("灯箱：");
        ui->statusLabel_6->setText("闭锁状态：");
        ui->statusLabel_7->setText("风扇：");
        ui->statusLabel_8->setText("按钮报警状态");
        ui->statusLabel_9->setText("切割报警状态：");
        ui->statusLabel_10->setText("温度报警状态：");
        ui->statusLabel_11->setText("震动报警状态：");
        ui->statusLabel_12->setText("水浸报警状态：");
        ui->statusLabel_13->setText("烟雾报警状态：");
        ui->statusLabel_14->setText("玻璃破碎报警状态");
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

        ui->tableWidgetAlarmPara->setItem(7,0,new QTableWidgetItem(""));
        ui->tableWidgetAlarmPara->setItem(8,0,new QTableWidgetItem(""));
    }
}
//状态页面显示项目修改为加钞间界面
void frmMain::changeJCJText()
{
    if(windowStatus!=2)
    {
        windowStatus=2;
        //disconnect(ui->btnEnOrDisAbleFangHuCang,SIGNAL(clicked(bool)),this,SLOT(slotEanbleOrDisableSlave()));
        //connect(ui->btnEnOrDisAbleFangHuCang,SIGNAL(clicked(bool)),this,SLOT(btnEnOrDisAbleFangHuCang()));
        //设置加钞间参数标签可见
        ui->tabWidgetSlavePara->clear();
        ui->tabWidgetSlavePara->addTab(ui->tabSlaveStatus,"运行状态");
        ui->tabWidgetSlavePara->addTab(ui->jcjTabSlavePara,"控制参数");
        ui->tabWidgetSlavePara->addTab(ui->tabVoicePara,"语音状态");
        ui->tabWidgetSlavePara->addTab(ui->tabLedPara,"LED参数");
        ui->tabWidgetSlavePara->addTab(ui->tabAlarmPara,"报警参数");
        ui->tabWidgetSlavePara->addTab(ui->jcjtabUserPara,"用户参数");
        //ui->tabWidgetSlavePara->setCurrentIndex(0);

        ui->btnOpenDoor->setStyleSheet("background-color:rgba(0,0,0,0);border-style:none;");
        ui->btnOpenDoor->setText("");
        ui->btnOpenDoor->setEnabled(false);

        ui->statusLabel_1->setText("工作状态：");
        ui->statusLabel_2->setText("门状态：");
        ui->statusLabel_3->setText("照明灯：");
        ui->statusLabel_4->setText("设防状态：");
        ui->statusLabel_5->setText("非法开门状态：");
        ui->statusLabel_6->setText("闭锁状态：");
        ui->statusLabel_7->setText("非法入侵状态：");
        ui->statusLabel_8->setText("按钮报警状态：");
        ui->statusLabel_9->setText("切割报警状态：");
        ui->statusLabel_10->setText("温度报警状态：");
        ui->statusLabel_11->setText("震动报警状态：");
        ui->statusLabel_12->setText("水浸报警状态：");
        ui->statusLabel_13->setText("烟雾报警状态：");
        ui->statusLabel_14->setText("玻璃破碎报警状态：");
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

        ui->tableWidgetAlarmPara->setItem(7,0,new QTableWidgetItem("是否启用非法开门报警"));
        ui->tableWidgetAlarmPara->setItem(8,0,new QTableWidgetItem("是否启用非法入侵报警"));
        ui->tableWidgetAlarmPara->item(7,0)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
        ui->tableWidgetAlarmPara->item(8,0)->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    }
}
//刷新状态
void frmMain::slotupdateSlaveStatusDisplay(paraData* pdata)
{
    if(pdata)
    {
        switch (pdata->slaveType)
        {
        case 0:/****************************防护舱******************************/
        {
            changeFHCText();
            if(pdata)
            {
                //根据版本号是否显示报警状态和报警设置页面
                static quint8 ver = 0;
                ver = pdata->secondVer;
                switch(ver)
                {
                case 7:
                    ui->tabWidgetSlavePara->setTabEnabled(4,false);
                    ui->lbIconTempAlarm->setStyleSheet("background-color:rgba(0,0,0,0);border-style:none;");
                    ui->statusLabel_10->setText("");
                    ui->lbIconTempAlarm->setText("");
                    ui->statusLabel_11->setVisible(false);
                    ui->statusLabel_13->setVisible(false);
                    ui->statusLabel_14->setVisible(false);
                    ui->statusLabel_12->setVisible(false);
                    ui->lbIconZhenDongAlarm->setVisible(false);
                    ui->lbIconYanWuAlarm->setVisible(false);
                    ui->lbIconShuiQinAlarm->setVisible(false);
                    ui->lbIconBoLiAlarm->setVisible(false);
                    break;
                case 8:
                    ui->tabWidgetSlavePara->setTabEnabled(4,true);
                    ui->lbIconTempAlarm->setStyleSheet("");
                    ui->statusLabel_10->setText("温度报警状态:");
                    ui->lbIconTempAlarm->setText("");
                    ui->statusLabel_11->setVisible(true);
                    ui->statusLabel_13->setVisible(true);
                    ui->statusLabel_14->setVisible(true);
                    ui->statusLabel_12->setVisible(true);
                    ui->lbIconZhenDongAlarm->setVisible(true);
                    ui->lbIconYanWuAlarm->setVisible(true);
                    ui->lbIconShuiQinAlarm->setVisible(true);
                    ui->lbIconBoLiAlarm->setVisible(true);
                    break;
                default:
                    ui->tabWidgetSlavePara->setTabEnabled(4,false);
                    ui->lbIconTempAlarm->setStyleSheet("background-color:rgba(0,0,0,0);border-style:none;");
                    ui->statusLabel_10->setText("");
                    ui->lbIconTempAlarm->setText("");
                    ui->statusLabel_11->setVisible(false);
                    ui->statusLabel_13->setVisible(false);
                    ui->statusLabel_14->setVisible(false);
                    ui->statusLabel_12->setVisible(false);
                    ui->lbIconZhenDongAlarm->setVisible(false);
                    ui->lbIconYanWuAlarm->setVisible(false);
                    ui->lbIconShuiQinAlarm->setVisible(false);
                    ui->lbIconBoLiAlarm->setVisible(false);
                    break;
                }
                //根据连接状态使能或不使能设置按钮
                switch(pdata->slaveStatus.connectStatus)
                {
                case 0:
                    disconnect(connectingGif, SIGNAL(frameChanged(int)), this, SLOT(connectIconChanged()));
                    connectingGif->stop();
                    ui->tbStatus->setIcon(iconDisConnected);
                    ui->tbStatus->setText("未连接");
                    //设置参数按钮全部失效
                    ui->btnSetZhaoMingPrar->setEnabled(false);
                    ui->btnSetAlarmPara->setEnabled(false);
                    ui->btnSetFangHuCangPara->setEnabled(false);
                    ui->btnReadFailLockSound->setEnabled(false);
                    ui->btnReadHySound->setEnabled(false);
                    ui->btnReadLaDoorSound->setEnabled(false);
                    ui->btnReadMsg->setEnabled(false);
                    ui->btnReadOpenDoorSound->setEnabled(false);
                    ui->btnReadOutDoorSound->setEnabled(false);
                    ui->btnReadSyzSound->setEnabled(false);
                    ui->btnReadTimeoutWarnSound->setEnabled(false);
                    ui->btnReadWhzSound->setEnabled(false);
                    ui->btnChange0->setEnabled(false);
                    ui->btnChange1->setEnabled(false);
                    ui->btnChange2->setEnabled(false);
                    ui->btnTime->setEnabled(false);
                    ui->btnOpenDoor->setEnabled(false);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(false);
                    ui->btnLock->setEnabled(false);
                    ui->btnChangeFailLockSound->setEnabled(false);
                    ui->btnChangeHySound->setEnabled(false);
                    ui->btnChangeLaDoorSound->setEnabled(false);
                    ui->btnChangeOpenDoorSound->setEnabled(false);
                    ui->btnChangeOutDoorSound->setEnabled(false);
                    ui->btnChangeSyzSound->setEnabled(false);
                    ui->btnChangeTimeouTiShiSound->setEnabled(false);
                    ui->btnChangeTimeoutWarnSound->setEnabled(false);
                    ui->btnChangeWhzDoorSound->setEnabled(false);
                    ui->btnVoiceOk->setEnabled(false);
                    break;
                case 1:
                    disconnect(connectingGif, SIGNAL(frameChanged(int)), this, SLOT(connectIconChanged()));
                    connectingGif->stop();
                    ui->tbStatus->setIcon(iconConnected);
                    ui->tbStatus->setText("连接正常");
                    //设置参数按钮全部生效
                    ui->btnSetZhaoMingPrar->setEnabled(true);
                    ui->btnSetAlarmPara->setEnabled(true);
                    ui->btnSetFangHuCangPara->setEnabled(true);
                    ui->btnReadFailLockSound->setEnabled(true);
                    ui->btnReadHySound->setEnabled(true);
                    ui->btnReadLaDoorSound->setEnabled(true);
                    ui->btnReadMsg->setEnabled(true);
                    ui->btnReadOpenDoorSound->setEnabled(true);
                    ui->btnReadOutDoorSound->setEnabled(true);
                    ui->btnReadSyzSound->setEnabled(true);
                    ui->btnReadTimeoutWarnSound->setEnabled(true);
                    ui->btnReadWhzSound->setEnabled(true);
                    ui->btnChange0->setEnabled(true);
                    ui->btnChange1->setEnabled(true);
                    ui->btnChange2->setEnabled(true);
                    ui->btnTime->setEnabled(true);
                    ui->btnOpenDoor->setEnabled(true);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(true);
                    ui->btnLock->setEnabled(true);
                    ui->btnChangeFailLockSound->setEnabled(true);
                    ui->btnChangeHySound->setEnabled(true);
                    ui->btnChangeLaDoorSound->setEnabled(true);
                    ui->btnChangeOpenDoorSound->setEnabled(true);
                    ui->btnChangeOutDoorSound->setEnabled(true);
                    ui->btnChangeSyzSound->setEnabled(true);
                    ui->btnChangeTimeouTiShiSound->setEnabled(true);
                    ui->btnChangeTimeoutWarnSound->setEnabled(true);
                    ui->btnChangeWhzDoorSound->setEnabled(true);
                    ui->btnVoiceOk->setEnabled(true);
                    break;
                case 2:
                    ui->tbStatus->setIcon(iconConnecting);
                    ui->tbStatus->setText("正在连接");
                    //动态图更新
                    connect(connectingGif, SIGNAL(frameChanged(int)), this, SLOT(connectIconChanged()));
                    connectingGif->start();
                    //设置参数按钮全部失效
                    ui->btnSetZhaoMingPrar->setEnabled(false);
                    ui->btnSetAlarmPara->setEnabled(false);
                    ui->btnSetFangHuCangPara->setEnabled(false);
                    ui->btnReadFailLockSound->setEnabled(false);
                    ui->btnReadHySound->setEnabled(false);
                    ui->btnReadLaDoorSound->setEnabled(false);
                    ui->btnReadMsg->setEnabled(false);
                    ui->btnReadOpenDoorSound->setEnabled(false);
                    ui->btnReadOutDoorSound->setEnabled(false);
                    ui->btnReadSyzSound->setEnabled(false);
                    ui->btnReadTimeoutWarnSound->setEnabled(false);
                    ui->btnReadWhzSound->setEnabled(false);
                    ui->btnChange0->setEnabled(false);
                    ui->btnChange1->setEnabled(false);
                    ui->btnChange2->setEnabled(false);
                    ui->btnTime->setEnabled(false);
                    ui->btnOpenDoor->setEnabled(false);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(false);
                    ui->btnLock->setEnabled(false);
                    ui->btnChangeFailLockSound->setEnabled(false);
                    ui->btnChangeHySound->setEnabled(false);
                    ui->btnChangeLaDoorSound->setEnabled(false);
                    ui->btnChangeOpenDoorSound->setEnabled(false);
                    ui->btnChangeOutDoorSound->setEnabled(false);
                    ui->btnChangeSyzSound->setEnabled(false);
                    ui->btnChangeTimeouTiShiSound->setEnabled(false);
                    ui->btnChangeTimeoutWarnSound->setEnabled(false);
                    ui->btnChangeWhzDoorSound->setEnabled(false);
                    ui->btnVoiceOk->setEnabled(false);
                    break;
                default:
                    ui->tbStatus->setIcon(iconDisConnected);
                    ui->tbStatus->setText("未连接");
                    //设置参数按钮全部失效
                    ui->btnSetZhaoMingPrar->setEnabled(false);
                    ui->btnSetAlarmPara->setEnabled(false);
                    ui->btnSetFangHuCangPara->setEnabled(false);
                    ui->btnReadFailLockSound->setEnabled(false);
                    ui->btnReadHySound->setEnabled(false);
                    ui->btnReadLaDoorSound->setEnabled(false);
                    ui->btnReadMsg->setEnabled(false);
                    ui->btnReadOpenDoorSound->setEnabled(false);
                    ui->btnReadOutDoorSound->setEnabled(false);
                    ui->btnReadSyzSound->setEnabled(false);
                    ui->btnReadTimeoutWarnSound->setEnabled(false);
                    ui->btnReadWhzSound->setEnabled(false);
                    ui->btnChange0->setEnabled(false);
                    ui->btnChange1->setEnabled(false);
                    ui->btnChange2->setEnabled(false);
                    ui->btnTime->setEnabled(false);
                    ui->btnOpenDoor->setEnabled(false);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(false);
                    ui->btnLock->setEnabled(false);
                    ui->btnChangeFailLockSound->setEnabled(false);
                    ui->btnChangeHySound->setEnabled(false);
                    ui->btnChangeLaDoorSound->setEnabled(false);
                    ui->btnChangeOpenDoorSound->setEnabled(false);
                    ui->btnChangeOutDoorSound->setEnabled(false);
                    ui->btnChangeSyzSound->setEnabled(false);
                    ui->btnChangeTimeouTiShiSound->setEnabled(false);
                    ui->btnChangeTimeoutWarnSound->setEnabled(false);
                    ui->btnChangeWhzDoorSound->setEnabled(false);
                    ui->btnVoiceOk->setEnabled(false);
                    break;
                }
                if(pdata->slaveStatus.connectStatus == 1)
                {
                    ui->lbTemperature->setText(QString::number((float)pdata->slaveStatus.temperature/10));
                    ui->lbShidu->setText(QString::number((float)pdata->slaveStatus.humidity/10));
                    //门状态

                    if(pdata->slaveStatus.gateStatus)
                    {
                        ui->lbIconGateStatus->setStyleSheet(gateOpen);
                    }
                    else
                    {
                        ui->lbIconGateStatus->setStyleSheet(gateClose);
                    }
                    //照明状态
                    ui->lbIconZhaoMing->setStyleSheet("");
                    if(pdata->slaveStatus.zhaomingStatus)
                    {
                        ui->lbIconZhaoMing->setStyleSheet(zhaoMingOpen);
                    }
                    else
                    {
                        ui->lbIconZhaoMing->setStyleSheet(zhaoMingClose);
                    }
                    //灯箱状态
                    ui->lbIconDengXiang->setStyleSheet("");
                    if(pdata->slaveStatus.dengxiangStatus)
                    {
                        ui->lbIconDengXiang->setStyleSheet(dengXiangOpen);
                    }
                    else
                    {
                        ui->lbIconDengXiang->setStyleSheet(dengXiangClose);
                    }
                    //风扇状态
                    if(pdata->slaveStatus.fanStatus)
                    {
                        ui->lbIconFan->setStyleSheet("");
                        ui->lbIconFan->setMovie(movie);
                        movie->start();
                    }
                    else
                    {
                        movie->stop();
                        ui->lbIconFan->clear();
                        ui->lbIconFan->setStyleSheet(fanClose);
                    }
                    //按钮报警状态

                    if(pdata->slaveStatus.buttonAlarmStatus)
                    {
                        ui->lbIconBtnWarning->setStyleSheet(btnAlarm);
                    }
                    else
                    {
                        ui->lbIconBtnWarning->setStyleSheet(btnNormal);
                    }
                    //使用状态
                    ui->lbIconUseStatus->setStyleSheet("");
                    if(pdata->slaveStatus.usedStatus)
                    {
                        ui->lbIconUseStatus->setStyleSheet("");
                        ui->lbIconUseStatus->setMovie(usingGif);
                        usingGif->start();
                    }
                    else
                    {
                        usingGif->stop();
                        ui->lbIconUseStatus->clear();
                        //ui->lbIconUseStatus->setStyleSheet("");
                        ui->lbIconUseStatus->setStyleSheet(notUsingIcon);
                    }
                    //防护舱状态
                    ui->lbIconFangHuCangStatus->setStyleSheet("");
                    if(!pdata->slaveStatus.fanghucangStatus)
                    {
                        ui->lbIconFangHuCangStatus->setStyleSheet(fangHuCangUse);
                        ui->btnEnOrDisAbleFangHuCang->setText("停用");
                    }
                    else
                    {
                        ui->lbIconFangHuCangStatus->setStyleSheet(fangHuCangNotUse);
                        ui->btnEnOrDisAbleFangHuCang->setText("启用");
                    }

                    //闭锁状态
                    if(pdata->slaveStatus.lockStatus)
                    {
                        ui->lbIconLockStatus->setStyleSheet(closeLock);
                        ui->btnLock->setText("开锁");
                    }
                    else
                    {
                        ui->lbIconLockStatus->setStyleSheet(openLock);
                        ui->btnLock->setText("闭锁");
                    }
                    //切割报警状态
                    if(pdata->slaveStatus.cutAlarmStatus)
                    {
                        ui->lbIconQieGeWarning->setStyleSheet(qieGeAlarm);
                    }
                    else
                    {
                        ui->lbIconQieGeWarning->setStyleSheet(qieGeNormal);
                    }
                    if(pdata->getTwoMainVer() > 27)
                    {
                        //震动报警
                        if(pdata->slaveStatus.zhengDongAlarmStatus)
                        {
                            ui->lbIconZhenDongAlarm->setStyleSheet(zhenDongAlarm);
                        }
                        else
                        {
                            ui->lbIconZhenDongAlarm->setStyleSheet(zhenDongNormal);
                        }
                        //烟雾报警
                        if(pdata->slaveStatus.yanWuAlarmStatus)
                        {
                            ui->lbIconYanWuAlarm->setStyleSheet(yanWuAlarm);
                        }
                        else
                        {
                            ui->lbIconYanWuAlarm->setStyleSheet(yanWuNormal);
                        }
                        //玻璃破碎报警
                        if(pdata->slaveStatus.boLiAlarmStatus)
                        {
                            ui->lbIconBoLiAlarm->setStyleSheet(boLiAlarm);
                        }
                        else
                        {
                            ui->lbIconBoLiAlarm->setStyleSheet(boLiNormal);
                        }
                        //水浸报警
                        if(pdata->slaveStatus.shuiQinAlarmStatus)
                        {
                            ui->lbIconShuiQinAlarm->setStyleSheet(shuiQinAlarm);
                        }
                        else
                        {
                            ui->lbIconShuiQinAlarm->setStyleSheet(shuiQinNormal);
                        }
                        //温度报警
                        if(pdata->slaveStatus.tempAlarmStatus)
                        {
                            ui->lbIconTempAlarm->setStyleSheet(tempAlarm);
                        }
                        else
                        {
                            ui->lbIconTempAlarm->setStyleSheet(tempNormal);
                        }
                    }
                }
                else
                {
                    clearFormStatusDisplay();
                }
            }
            else
            {
                clearFormParaDisplay();
                clearFormStatusDisplay();
            }
        }
            break;
        case 1:/****************************加钞间******************************/
        {
            changeJCJText();
            if(pdata)
            {
                //显示报警设置页面
                ui->tabWidgetSlavePara->setTabEnabled(4,true);
                ui->lbIconTempAlarm->setStyleSheet("");
                ui->statusLabel_10->setText("温度报警状态:");
                ui->lbIconTempAlarm->setText("");
                ui->statusLabel_11->setVisible(true);
                ui->statusLabel_13->setVisible(true);
                ui->statusLabel_14->setVisible(true);
                ui->statusLabel_12->setVisible(true);
                ui->lbIconZhenDongAlarm->setVisible(true);
                ui->lbIconYanWuAlarm->setVisible(true);
                ui->lbIconShuiQinAlarm->setVisible(true);
                ui->lbIconBoLiAlarm->setVisible(true);

                //根据连接状态使能或不使能设置按钮
                switch(pdata->slaveStatus.connectStatus)
                {
                case 0:
                    disconnect(connectingGif, SIGNAL(frameChanged(int)), this, SLOT(connectIconChanged()));
                    connectingGif->stop();
                    ui->tbStatus->setIcon(iconDisConnected);
                    ui->tbStatus->setText("未连接");
                    //设置参数按钮全部失效
                    ui->btnSetZhaoMingPrar->setEnabled(false);
                    ui->btnSetAlarmPara->setEnabled(false);
                    ui->btnSetFangHuCangPara->setEnabled(false);
                    ui->btnReadFailLockSound->setEnabled(false);
                    ui->btnReadHySound->setEnabled(false);
                    ui->btnReadLaDoorSound->setEnabled(false);
                    ui->btnReadMsg->setEnabled(false);
                    ui->btnReadOpenDoorSound->setEnabled(false);
                    ui->btnReadOutDoorSound->setEnabled(false);
                    ui->btnReadSyzSound->setEnabled(false);
                    ui->btnReadTimeoutWarnSound->setEnabled(false);
                    ui->btnReadWhzSound->setEnabled(false);
                    ui->btnChange0->setEnabled(false);
                    ui->btnChange1->setEnabled(false);
                    ui->btnChange2->setEnabled(false);
                    ui->btnTime->setEnabled(false);
                    ui->btnOpenDoor->setEnabled(false);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(false);
                    ui->btnLock->setEnabled(false);
                    ui->btnChangeFailLockSound->setEnabled(false);
                    ui->btnChangeHySound->setEnabled(false);
                    ui->btnChangeLaDoorSound->setEnabled(false);
                    ui->btnChangeOpenDoorSound->setEnabled(false);
                    ui->btnChangeOutDoorSound->setEnabled(false);
                    ui->btnChangeSyzSound->setEnabled(false);
                    ui->btnChangeTimeouTiShiSound->setEnabled(false);
                    ui->btnChangeTimeoutWarnSound->setEnabled(false);
                    ui->btnChangeWhzDoorSound->setEnabled(false);
                    ui->btnVoiceOk->setEnabled(false);
                    break;
                case 1:
                    disconnect(connectingGif, SIGNAL(frameChanged(int)), this, SLOT(connectIconChanged()));
                    connectingGif->stop();
                    ui->tbStatus->setIcon(iconConnected);
                    ui->tbStatus->setText("连接正常");
                    //设置参数按钮全部生效
                    ui->btnSetZhaoMingPrar->setEnabled(true);
                    ui->btnSetAlarmPara->setEnabled(true);
                    ui->btnSetFangHuCangPara->setEnabled(true);
                    ui->btnReadFailLockSound->setEnabled(true);
                    ui->btnReadHySound->setEnabled(true);
                    ui->btnReadLaDoorSound->setEnabled(true);
                    ui->btnReadMsg->setEnabled(true);
                    ui->btnReadOpenDoorSound->setEnabled(true);
                    ui->btnReadOutDoorSound->setEnabled(true);
                    ui->btnReadSyzSound->setEnabled(true);
                    ui->btnReadTimeoutWarnSound->setEnabled(true);
                    ui->btnReadWhzSound->setEnabled(true);
                    ui->btnChange0->setEnabled(true);
                    ui->btnChange1->setEnabled(true);
                    ui->btnChange2->setEnabled(true);
                    ui->btnTime->setEnabled(true);
                    //ui->btnOpenDoor->setEnabled(true);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(true);
                    ui->btnLock->setEnabled(true);
                    ui->btnChangeFailLockSound->setEnabled(true);
                    ui->btnChangeHySound->setEnabled(true);
                    ui->btnChangeLaDoorSound->setEnabled(true);
                    ui->btnChangeOpenDoorSound->setEnabled(true);
                    ui->btnChangeOutDoorSound->setEnabled(true);
                    ui->btnChangeSyzSound->setEnabled(true);
                    ui->btnChangeTimeouTiShiSound->setEnabled(true);
                    ui->btnChangeTimeoutWarnSound->setEnabled(true);
                    ui->btnChangeWhzDoorSound->setEnabled(true);
                    ui->btnVoiceOk->setEnabled(true);
                    break;
                case 2:
                    ui->tbStatus->setIcon(iconConnecting);
                    ui->tbStatus->setText("正在连接");
                    //动态图更新
                    connect(connectingGif, SIGNAL(frameChanged(int)), this, SLOT(connectIconChanged()));
                    connectingGif->start();
                    //设置参数按钮全部失效
                    ui->btnSetZhaoMingPrar->setEnabled(false);
                    ui->btnSetAlarmPara->setEnabled(false);
                    ui->btnSetFangHuCangPara->setEnabled(false);
                    ui->btnReadFailLockSound->setEnabled(false);
                    ui->btnReadHySound->setEnabled(false);
                    ui->btnReadLaDoorSound->setEnabled(false);
                    ui->btnReadMsg->setEnabled(false);
                    ui->btnReadOpenDoorSound->setEnabled(false);
                    ui->btnReadOutDoorSound->setEnabled(false);
                    ui->btnReadSyzSound->setEnabled(false);
                    ui->btnReadTimeoutWarnSound->setEnabled(false);
                    ui->btnReadWhzSound->setEnabled(false);
                    ui->btnChange0->setEnabled(false);
                    ui->btnChange1->setEnabled(false);
                    ui->btnChange2->setEnabled(false);
                    ui->btnTime->setEnabled(false);
                    ui->btnOpenDoor->setEnabled(false);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(false);
                    ui->btnLock->setEnabled(false);
                    ui->btnChangeFailLockSound->setEnabled(false);
                    ui->btnChangeHySound->setEnabled(false);
                    ui->btnChangeLaDoorSound->setEnabled(false);
                    ui->btnChangeOpenDoorSound->setEnabled(false);
                    ui->btnChangeOutDoorSound->setEnabled(false);
                    ui->btnChangeSyzSound->setEnabled(false);
                    ui->btnChangeTimeouTiShiSound->setEnabled(false);
                    ui->btnChangeTimeoutWarnSound->setEnabled(false);
                    ui->btnChangeWhzDoorSound->setEnabled(false);
                    ui->btnVoiceOk->setEnabled(false);
                    break;
                default:
                    ui->tbStatus->setIcon(iconDisConnected);
                    ui->tbStatus->setText("未连接");
                    //设置参数按钮全部失效
                    ui->btnSetZhaoMingPrar->setEnabled(false);
                    ui->btnSetAlarmPara->setEnabled(false);
                    ui->btnSetFangHuCangPara->setEnabled(false);
                    ui->btnReadFailLockSound->setEnabled(false);
                    ui->btnReadHySound->setEnabled(false);
                    ui->btnReadLaDoorSound->setEnabled(false);
                    ui->btnReadMsg->setEnabled(false);
                    ui->btnReadOpenDoorSound->setEnabled(false);
                    ui->btnReadOutDoorSound->setEnabled(false);
                    ui->btnReadSyzSound->setEnabled(false);
                    ui->btnReadTimeoutWarnSound->setEnabled(false);
                    ui->btnReadWhzSound->setEnabled(false);
                    ui->btnChange0->setEnabled(false);
                    ui->btnChange1->setEnabled(false);
                    ui->btnChange2->setEnabled(false);
                    ui->btnTime->setEnabled(false);
                    ui->btnOpenDoor->setEnabled(false);
                    ui->btnEnOrDisAbleFangHuCang->setEnabled(false);
                    ui->btnLock->setEnabled(false);
                    ui->btnChangeFailLockSound->setEnabled(false);
                    ui->btnChangeHySound->setEnabled(false);
                    ui->btnChangeLaDoorSound->setEnabled(false);
                    ui->btnChangeOpenDoorSound->setEnabled(false);
                    ui->btnChangeOutDoorSound->setEnabled(false);
                    ui->btnChangeSyzSound->setEnabled(false);
                    ui->btnChangeTimeouTiShiSound->setEnabled(false);
                    ui->btnChangeTimeoutWarnSound->setEnabled(false);
                    ui->btnChangeWhzDoorSound->setEnabled(false);
                    ui->btnVoiceOk->setEnabled(false);
                    break;
                }
                if(pdata->slaveStatus.connectStatus == 1)
                {
                    ui->lbTemperature->setText(QString::number((float)pdata->slaveStatus.temperature/10));
                    ui->lbShidu->setText(QString::number((float)pdata->slaveStatus.humidity/10));
                    //工作状态
                    ui->lbIconUseStatus->setStyleSheet("");
                    switch (pdata->slaveStatus.workStatus)
                    {
                    case 0://正常状态
                        ui->lbIconUseStatus->setStyleSheet(iconCjcWorkNormalState);
                        break;
                    case 1://管理员状态
                        ui->lbIconUseStatus->setStyleSheet(iconCjcWorkEditAdminState);
                        break;
                    case 2://注册用户状态
                        ui->lbIconUseStatus->setStyleSheet(iconCjcWorkOperateMainCardState);
                        break;
                    case 3://删除用户状态
                        ui->lbIconUseStatus->setStyleSheet(iconCjcWorkOperateMainCardState);
                        break;
                    case 4://删除全部用户状态
                        ui->lbIconUseStatus->setStyleSheet(iconCjcWorkOperateMainCardState);
                        break;
                    case 5://修改管理员状态
                        ui->lbIconUseStatus->setStyleSheet(iconCjcWorkEditAdminState);
                        break;
                    }
                    //门状态
                    if(pdata->slaveStatus.gateStatus)
                    {
                        ui->lbIconGateStatus->setStyleSheet(gateOpen);
                    }
                    else
                    {
                        ui->lbIconGateStatus->setStyleSheet(gateClose);
                    }
                    //照明状态
                    ui->lbIconZhaoMing->setStyleSheet("");
                    if(pdata->slaveStatus.zhaomingStatus)
                    {
                        ui->lbIconZhaoMing->setStyleSheet(zhaoMingOpen);
                    }
                    else
                    {
                        ui->lbIconZhaoMing->setStyleSheet(zhaoMingClose);
                    }

                    //非法开门状态
                    ui->lbIconDengXiang->setStyleSheet("");
                    if(pdata->slaveStatus.doorCiAlarmStatus)
                    {
                        ui->lbIconDengXiang->setStyleSheet(iconCjcAlarmIllegalOpenDoorEnable);
                    }
                    else
                    {
                        ui->lbIconDengXiang->setStyleSheet(iconCjcAlarmIllegalOpenDoorDisable);
                    }

                    //非法入侵状态
                    if(pdata->slaveStatus.existManAlarmStatus)
                    {
                        ui->lbIconFan->setStyleSheet(iconCjcAlarmIllegalInEnable);
                    }
                    else
                    {
                        ui->lbIconFan->setStyleSheet(iconCjcAlarmIllegalInDisable);
                    }

                    //按钮报警状态

                    if(pdata->slaveStatus.buttonAlarmStatus)
                    {
                        ui->lbIconBtnWarning->setStyleSheet(btnAlarm);
                    }
                    else
                    {
                        ui->lbIconBtnWarning->setStyleSheet(btnNormal);
                    }

                    //设防状态
                    ui->lbIconFangHuCangStatus->setStyleSheet("");
                    switch (pdata->slaveStatus.alarmStatus)
                    {
                    case 0://设防状态
                        ui->lbIconFangHuCangStatus->setStyleSheet(iconCjcAlarmSetGuardState);
                        ui->btnEnOrDisAbleFangHuCang->setText("撤防");
                        break;
                    case 1://等待到撤防状态
                        ui->lbIconFangHuCangStatus->setStyleSheet(iconCjcAlarmWaitSetGuardState);
                        ui->btnEnOrDisAbleFangHuCang->setText("撤防");
                        break;
                    case 2://撤防状态
                        ui->lbIconFangHuCangStatus->setStyleSheet(iconCjcAlarmResetGuardState);
                        ui->btnEnOrDisAbleFangHuCang->setText("设防");
                        break;
                    case 3://等待到设防状态
                        ui->lbIconFangHuCangStatus->setStyleSheet(iconCjcAlarmWaitSetGuardState);
                        ui->btnEnOrDisAbleFangHuCang->setText("设防");
                        break;
                    case 4://处于报警状态
                        ui->lbIconFangHuCangStatus->setStyleSheet("iconCjcAlarmSetGuardState");
                        ui->btnEnOrDisAbleFangHuCang->setText("");
                        break;
                    }

                    //闭锁状态
                    if(pdata->slaveStatus.lockStatus)
                    {
                        ui->lbIconLockStatus->setStyleSheet(closeLock);
                        ui->btnLock->setText("开锁");
                    }
                    else
                    {
                        ui->lbIconLockStatus->setStyleSheet(openLock);
                        ui->btnLock->setText("闭锁");
                    }
                    //切割报警状态
                    if(pdata->slaveStatus.cutAlarmStatus)
                    {
                        ui->lbIconQieGeWarning->setStyleSheet(qieGeAlarm);
                    }
                    else
                    {
                        ui->lbIconQieGeWarning->setStyleSheet(qieGeNormal);
                    }

                    //震动报警
                    if(pdata->slaveStatus.zhengDongAlarmStatus)
                    {
                        ui->lbIconZhenDongAlarm->setStyleSheet(zhenDongAlarm);
                    }
                    else
                    {
                        ui->lbIconZhenDongAlarm->setStyleSheet(zhenDongNormal);
                    }
                    //烟雾报警
                    if(pdata->slaveStatus.yanWuAlarmStatus)
                    {
                        ui->lbIconYanWuAlarm->setStyleSheet(yanWuAlarm);
                    }
                    else
                    {
                        ui->lbIconYanWuAlarm->setStyleSheet(yanWuNormal);
                    }
                    //玻璃破碎报警
                    if(pdata->slaveStatus.boLiAlarmStatus)
                    {
                        ui->lbIconBoLiAlarm->setStyleSheet(boLiAlarm);
                    }
                    else
                    {
                        ui->lbIconBoLiAlarm->setStyleSheet(boLiNormal);
                    }
                    //水浸报警
                    if(pdata->slaveStatus.shuiQinAlarmStatus)
                    {
                        ui->lbIconShuiQinAlarm->setStyleSheet(shuiQinAlarm);
                    }
                    else
                    {
                        ui->lbIconShuiQinAlarm->setStyleSheet(shuiQinNormal);
                    }
                    //温度报警
                    if(pdata->slaveStatus.tempAlarmStatus)
                    {
                        ui->lbIconTempAlarm->setStyleSheet(tempAlarm);
                    }
                    else
                    {
                        ui->lbIconTempAlarm->setStyleSheet(tempNormal);
                    }
                }
                else
                {
                    clearFormStatusDisplay();
                }
            }
            else
            {
                clearFormParaDisplay();
                clearFormStatusDisplay();
            }

        }
            break;
        case 0xff:
            break;
        default:
            break;
        }
    }
}
//刷新设备参数
void frmMain::slotupdateSlaveParaDisplay(paraData* pdata)
{
    if(pdata)
    {
        //设备参数显示
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(pdata->name));
        ui->tableWidget->setItem(1,1,new QTableWidgetItem(pdata->getSlaveType()));
        ui->tableWidget->setItem(2,1,new QTableWidgetItem(pdata->version));
        QString slaveMac = pdata->net.macAddr.toUpper();
        ui->tableWidget->setItem(3,1,new QTableWidgetItem(slaveMac));
        ui->tableWidget->setItem(4,1,new QTableWidgetItem(pdata->net.ip));
        ui->tableWidget->setItem(5,1,new QTableWidgetItem(pdata->net.networkModel?"客户端模式":"服务器模式"));
    }
}
//刷新照明参数
void frmMain::slotupdateSlaveZhaoMingParaDisplay(paraData* pdata)
{
    if(pdata)
    {
        ui->tablewidgetZhaoMing->setItem(0,1,new QTableWidgetItem(pdata->zhaoMing.getModel()));
        QString temp =QString::number(pdata->zhaoMing.startHour)+"点"+QString::number(pdata->zhaoMing.startMinute)+"分"+"到"+QString::number(pdata->zhaoMing.endHour)+"点"+QString::number(pdata->zhaoMing.endMinute)+"分";
        ui->tablewidgetZhaoMing->setItem(1,1,new QTableWidgetItem(temp));
        ui->tablewidgetZhaoMing->setItem(2,1,new QTableWidgetItem(pdata->zhaoMing.manEnable?"启用":"不启用"));
    }
}
//刷新防护舱参数
void frmMain::slotupdateSlaveFangHuCangParaDisplay(paraData* pdata)
{
    if(pdata)
    {
        switch (pdata->slaveType)
        {
        case 0://防护舱
        {
            ui->tableWidgetFangHuCang->setItem(0,1,new QTableWidgetItem(pdata->fangHuCang.getlockModel()));
            ui->tableWidgetFangHuCang->setItem(1,1,new QTableWidgetItem(QString::number(pdata->fangHuCang.openLockTime)+"秒钟"));
            ui->tableWidgetFangHuCang->setItem(2,1,new QTableWidgetItem(pdata->fangHuCang.signalModel?"常开":"常闭"));
            ui->tableWidgetFangHuCang->setItem(3,1,new QTableWidgetItem(pdata->fangHuCang.peopleEquModel?"被动":"主动"));
            ui->tableWidgetFangHuCang->setItem(4,1,new QTableWidgetItem(pdata->fangHuCang.kongCangLockorNot?"是":"否"));
            ui->tableWidgetFangHuCang->setItem(5,1,new QTableWidgetItem(QString::number(pdata->fangHuCang.fanRunTemperature)+"度"));

            ui->tableWidgetFangHuCang->setItem(0,3,new QTableWidgetItem(QString::number(pdata->fangHuCang.noManOpenLockTime)+"秒钟"));
            ui->tableWidgetFangHuCang->setItem(1,3,new QTableWidgetItem(QString::number(pdata->fangHuCang.OptBussinessTime)+"分钟"));
            ui->tableWidgetFangHuCang->setItem(2,3,new QTableWidgetItem(QString::number(pdata->fangHuCang.timeOutRemind)+"秒钟"));
            ui->tableWidgetFangHuCang->setItem(3,3,new QTableWidgetItem(QString::number(pdata->fangHuCang.warnningDelayTime)+"秒钟"));
            ui->tableWidgetFangHuCang->setItem(4,3,new QTableWidgetItem(pdata->fangHuCang.fangQiewarnning?"启动":"不启用"));
        }
            break;
        case 1://加钞间
        {
            ui->tableWidgeCjc->setItem(0,1,new QTableWidgetItem(QString::number(pdata->fangHuCang.openLockTime)+"秒钟"));
            ui->tableWidgeCjc->setItem(1,1,new QTableWidgetItem(QString::number(pdata->fangHuCang.setGuardDelayTime)+"秒钟"));
            ui->tableWidgeCjc->setItem(2,1,new QTableWidgetItem(QString::number(pdata->fangHuCang.userNum)+"+1"));
            ui->tableWidgeCjc->setItem(3,1,new QTableWidgetItem(pdata->fangHuCang.getInDoorModel()));
            ui->tableWidgeCjc->setItem(4,1,new QTableWidgetItem(pdata->fangHuCang.getOutDoorModel()));
            ui->tableWidgeCjc->setItem(5,1,new QTableWidgetItem(pdata->fangHuCang.systemAlarmStatus?"设防":"撤防"));
            ui->tableWidgeCjc->setItem(6,1,new QTableWidgetItem(pdata->fangHuCang.isMonitorOrNot?"有人值守":"无人值守"));
        }
            break;
        case 0xff://bootloader
            break;
        }


    }
}
//刷新led内容参数
void frmMain::slotupdateSlaveLedParaDisplay(paraData* pdata)
{
    if(pdata)
    {
        ui->txtBrowse0->setText(pdata->ledText[0]);
        ui->txtBrowse1->setText(pdata->ledText[1]);
        ui->txtBrowse2->setText(pdata->ledText[2]);
    }
}
//刷新报警参数
void frmMain::slotupdateSlaveAlarmParaDisplay(paraData* pdata)
{
    if(pdata)
    {
        switch(pdata->slaveType)
        {
        case 0:
            ui->tableWidgetAlarmPara->setItem(7,1,new QTableWidgetItem(""));
            ui->tableWidgetAlarmPara->setItem(8,1,new QTableWidgetItem(""));
            break;
        case 1:
            ui->tableWidgetAlarmPara->setItem(7,1,new QTableWidgetItem(pdata->alarmPara.doorCiAlarmEnable?"启用":"不启用"));
            ui->tableWidgetAlarmPara->setItem(8,1,new QTableWidgetItem(pdata->alarmPara.existManAlarmEnable?"启用":"不启用"));
            break;
        case 0xff:
            break;
        default:
            break;
        }
        //公共报警参数
        ui->tableWidgetAlarmPara->setItem(0,1,new QTableWidgetItem(pdata->alarmPara.btnAlarmEnable?"启用":"不启用"));
        ui->tableWidgetAlarmPara->setItem(1,1,new QTableWidgetItem(pdata->alarmPara.cutAlarmEnable?"启用":"不启用"));
        ui->tableWidgetAlarmPara->setItem(2,1,new QTableWidgetItem(pdata->alarmPara.zhengDongAlarmEnable?"启用":"不启用"));
        ui->tableWidgetAlarmPara->setItem(3,1,new QTableWidgetItem(pdata->alarmPara.yanWuAlarmEnable?"启用":"不启用"));
        ui->tableWidgetAlarmPara->setItem(4,1,new QTableWidgetItem(pdata->alarmPara.boLiAlarmEnable?"启用":"不启用"));
        ui->tableWidgetAlarmPara->setItem(5,1,new QTableWidgetItem(pdata->alarmPara.shuiQinAlarmEnable?"启用":"不启用"));
        ui->tableWidgetAlarmPara->setItem(6,1,new QTableWidgetItem(pdata->alarmPara.tempAlarmEnable?"启用":"不启用"));
    }
}
void frmMain::getSlaveSoundName(paraData* pdata)
{
    quint8 rowcount = voiceModel->rowCount();
    for(int i=0;i<9;i++)
    {
        pdata->soundName[i]= "未知";//如果没有找到对应的声音名称则显示 未知

        for(int j=0;j<rowcount;j++)
        {
            QModelIndex index = voiceModel->index(j,1);//获取md5

            if(pdata->soundMd5[i].toHex() == voiceModel->data(index).toString())
            {
                QModelIndex index1 = voiceModel->index(j,0);//获取名称
                pdata->soundName[i]= voiceModel->data(index1).toString();
                break;
            }
        }
    }
}
//刷新voice内容参数
void frmMain::slotupdateSlaveVoiceParaDisplay(paraData* pdata)
{
    if(pdata)
    {
        getSlaveSoundName(pdata);
        //获取声音名称
        ui->hsVoice->setValue(pdata->soundVolume);
        ui->le0->setText(pdata->soundName[0]);
        ui->le1->setText(pdata->soundName[1]);
        ui->le2->setText(pdata->soundName[2]);
        ui->le3->setText(pdata->soundName[3]);
        ui->le4->setText(pdata->soundName[4]);
        ui->le5->setText(pdata->soundName[5]);
        ui->le6->setText(pdata->soundName[6]);
        ui->le7->setText(pdata->soundName[7]);
        ui->le8->setText(pdata->soundName[8]);

    }
}
void frmMain::updateParaDisplay(Equipment *temp)
{
    //QLOG_INFO()<<"form display update";
    if(temp)
    {
        currentSlave = temp;
        paraData* pdata=temp->GetEquAll();

        //时间显示
        slotupdateSlaveTimeDisplay(pdata);
        slotupdateSlaveStatusDisplay(pdata);
        slotupdateSlaveParaDisplay(pdata);
        slotupdateSlaveZhaoMingParaDisplay(pdata);
        slotupdateSlaveFangHuCangParaDisplay(pdata);
        slotupdateSlaveLedParaDisplay(pdata);
        slotupdateSlaveVoiceParaDisplay(pdata);
        slotupdateSlaveAlarmParaDisplay(pdata);


    }
    else
    {
        clearFormParaDisplay();
        clearFormStatusDisplay();
        ui->tbStatus->setIcon(iconDisConnected);
        ui->tbStatus->setText("未连接");


    }

}
void frmMain::clearFormStatusDisplay(void)
{

    //清除状态显示
    movie->stop();
    ui->lbIconFan->setMovie(NULL);
    usingGif->stop();
    ui->lbIconUseStatus->setMovie(NULL);
    ui->lbIconFangHuCangStatus->setMovie(NULL);

    ui->lbTemperature->setText("未知");
    ui->lbShidu->setText("未知");
    ui->btnLock->setText("开锁");
    ui->lbIconGateStatus->setText("");
    ui->lbIconGateStatus->setStyleSheet(doorStatusNull);

    ui->lbIconZhaoMing->setText("");
    ui->lbIconZhaoMing->setStyleSheet(statusNull);

    ui->lbIconDengXiang->setText("");
    ui->lbIconDengXiang->setStyleSheet(statusNull);

    ui->lbIconBtnWarning->setText("");
    ui->lbIconBtnWarning->setStyleSheet(statusNull);

    ui->lbIconFan->setText("");
    ui->lbIconFan->setStyleSheet(statusNull);

    ui->lbIconFangHuCangStatus->setText("");
    ui->lbIconFangHuCangStatus->setStyleSheet(statusNull);

    ui->lbIconUseStatus->setText("");
    ui->lbIconUseStatus->setStyleSheet(statusNull);

    ui->lbIconLockStatus->setText("");
    ui->lbIconLockStatus->setStyleSheet(statusNull);

    ui->lbIconQieGeWarning->setText("");
    ui->lbIconQieGeWarning->setStyleSheet(statusNull);
    if(currentSlave)
    {
        if(currentSlave->GetEquAll()->getTwoMainVer() > 27||(currentSlave->GetEquAll()->slaveType == 1))
        {
            ui->lbIconZhenDongAlarm->setText("");
            ui->lbIconZhenDongAlarm->setStyleSheet(statusNull);

            ui->lbIconYanWuAlarm->setText("");
            ui->lbIconYanWuAlarm->setStyleSheet(statusNull);

            ui->lbIconShuiQinAlarm->setText("");
            ui->lbIconShuiQinAlarm->setStyleSheet(statusNull);

            ui->lbIconBoLiAlarm->setText("");
            ui->lbIconBoLiAlarm->setStyleSheet(statusNull);

            ui->lbIconTempAlarm->setText("");
            ui->lbIconTempAlarm->setStyleSheet(statusNull);
        }
    }

}
void frmMain::clearFormParaDisplay(void)
{
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(3,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(4,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(5,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(6,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(7,1,new QTableWidgetItem(""));

    ui->tableWidgetFangHuCang->setItem(0,1,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(1,1,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(2,1,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(3,1,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(4,1,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(5,1,new QTableWidgetItem(""));

    ui->tableWidgetFangHuCang->setItem(0,3,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(1,3,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(2,3,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(3,3,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(4,3,new QTableWidgetItem(""));
    ui->tableWidgetFangHuCang->setItem(5,3,new QTableWidgetItem(""));

    ui->tablewidgetZhaoMing->setItem(0,1,new QTableWidgetItem(""));
    ui->tablewidgetZhaoMing->setItem(1,1,new QTableWidgetItem(""));
    ui->tablewidgetZhaoMing->setItem(2,1,new QTableWidgetItem(""));
    //清除报警是否启用参数表
    ui->tableWidgetAlarmPara->setItem(0,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(1,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(2,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(3,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(4,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(5,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(6,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(7,1,new QTableWidgetItem(""));
    ui->tableWidgetAlarmPara->setItem(8,1,new QTableWidgetItem(""));
    //清除声音名称
    ui->hsVoice->setValue(0);
    ui->lbVoice->setText("0");
    ui->lbVoice->setText("");
    ui->le0->setText("");
    ui->le1->setText("");
    ui->le2->setText("");
    ui->le3->setText("");
    ui->le4->setText("");
    ui->le5->setText("");
    ui->le6->setText("");
    ui->le7->setText("");
    ui->le8->setText("");
    //清除led内容
    ui->txtBrowse0->setText("");
    ui->txtBrowse1->setText("");
    ui->txtBrowse2->setText("");
}
void frmMain::expanded(QModelIndex index)
{
    bool bExpanded = ui->treeView->isExpanded(index);
    if (!bExpanded)
    {
        ui->treeView->expand(index);
    }
    else
    {
        ui->treeView->collapse(index);
    }
}
//插入数据库设备表
void frmMain::Insert_TSlave_Sql(Equipment *temp)
{
    if(checkExist("TSlave","Mac",temp->equParaData.net.macAddr))
    {
        return;
    }
    //插入新项目
    query->prepare(insert_TSlave_sql);
    //mac地址
    query->addBindValue(temp->equParaData.net.macAddr);
    //设备类型
    query->addBindValue(temp->equParaData.slaveType);
    //版本号
    query->addBindValue(temp->equParaData.version);
    //名称
    query->addBindValue(temp->equParaData.name);
    //IP地址
    query->addBindValue(temp->equParaData.net.ip);
    //是否自动连接
    query->addBindValue(1);
    //在树中的index
    query->addBindValue(temp->indexInTree);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
void frmMain::Insert_TNetWork_Sql(Equipment *temp)
{
    if(checkExist("SlaveNetWork","Mac",temp->equParaData.net.macAddr))
    {
        return;
    }
    //插入新项目
    query->prepare(insert_TNetWork_sql);
    //mac地址
    query->addBindValue(temp->equParaData.net.macAddr);
    //Ip
    query->addBindValue(temp->equParaData.net.ip);
    //网关gateway
    query->addBindValue(temp->equParaData.net.gateWay);
    //子网掩码
    query->addBindValue(temp->equParaData.net.subnetMask);
    //网络模式networkmodel
    query->addBindValue(temp->equParaData.net.networkModel);
    //远程ip
    query->addBindValue(temp->equParaData.net.remoteIp);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//插入设备参数表
void frmMain::Insert_TParamater_Sql(Equipment *temp)
{
    if(checkExist("TParamater","Mac",temp->equParaData.net.macAddr))
    {
        return;
    }
    //插入设备参数表
    query->prepare(insert_TParamater_sql);
    //Mac地址
    query->addBindValue(temp->equParaData.net.macAddr);
    //防护舱参数
    query->addBindValue(temp->equParaData.fangHuCang.openLockTime);
    query->addBindValue(temp->equParaData.fangHuCang.OptBussinessTime);
    query->addBindValue(temp->equParaData.fangHuCang.timeOutRemind);
    query->addBindValue(temp->equParaData.fangHuCang.warnningDelayTime);
    query->addBindValue(temp->equParaData.fangHuCang.noManOpenLockTime);
    query->addBindValue(temp->equParaData.fangHuCang.fanRunTemperature);
    query->addBindValue(temp->equParaData.fangHuCang.signalModel);
    query->addBindValue(temp->equParaData.fangHuCang.peopleEquModel);
    query->addBindValue(temp->equParaData.fangHuCang.lockModel);
    query->addBindValue(temp->equParaData.fangHuCang.kongCangLockorNot);
    query->addBindValue(temp->equParaData.fangHuCang.fangQiewarnning);
    //写入照明参数
    query->addBindValue(temp->equParaData.zhaoMing.Model);
    query->addBindValue(temp->equParaData.zhaoMing.startHour);
    query->addBindValue(temp->equParaData.zhaoMing.startMinute);
    query->addBindValue(temp->equParaData.zhaoMing.endHour);
    query->addBindValue(temp->equParaData.zhaoMing.endMinute);
    query->addBindValue(temp->equParaData.zhaoMing.manEnable);
    //写入LED节目参数
    query->addBindValue(temp->equParaData.ledText[0]);
    query->addBindValue(temp->equParaData.ledText[1]);
    query->addBindValue(temp->equParaData.ledText[2]);
    //音乐参数
    query->addBindValue(temp->equParaData.soundVolume);
    query->addBindValue(temp->equParaData.soundMd5[0].toHex());
    query->addBindValue(temp->equParaData.soundMd5[1].toHex());
    query->addBindValue(temp->equParaData.soundMd5[2].toHex());
    query->addBindValue(temp->equParaData.soundMd5[3].toHex());
    query->addBindValue(temp->equParaData.soundMd5[4].toHex());
    query->addBindValue(temp->equParaData.soundMd5[5].toHex());
    query->addBindValue(temp->equParaData.soundMd5[6].toHex());
    query->addBindValue(temp->equParaData.soundMd5[7].toHex());    
    query->addBindValue(temp->equParaData.soundMd5[8].toHex());
    //更新报警设置参数
    query->addBindValue(temp->equParaData.alarmPara.btnAlarmEnable);
    query->addBindValue(temp->equParaData.alarmPara.cutAlarmEnable);
    query->addBindValue(temp->equParaData.alarmPara.zhengDongAlarmEnable);
    query->addBindValue(temp->equParaData.alarmPara.yanWuAlarmEnable);
    query->addBindValue(temp->equParaData.alarmPara.boLiAlarmEnable);
    query->addBindValue(temp->equParaData.alarmPara.shuiQinAlarmEnable);
    query->addBindValue(temp->equParaData.alarmPara.tempAlarmEnable);
    //加钞间新增参数
    query->addBindValue(temp->equParaData.fangHuCang.userNum);
    query->addBindValue(temp->equParaData.fangHuCang.systemAlarmStatus);
    query->addBindValue(temp->equParaData.fangHuCang.setGuardDelayTime);
    query->addBindValue(temp->equParaData.fangHuCang.isMonitorOrNot);
    query->addBindValue(temp->equParaData.fangHuCang.inDoorModel);
    query->addBindValue(temp->equParaData.fangHuCang.outDoorModel);
    query->addBindValue(temp->equParaData.alarmPara.doorCiAlarmEnable);
    query->addBindValue(temp->equParaData.alarmPara.existManAlarmEnable);
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//插入设备参数表
void frmMain::Insert_TMparamater_Sql(QString name,paraData *pdat)
{
    if(checkExist("TMparamater","Name",name))
    {
        return;
    }
    //插入设备参数表
    query->prepare(insert_TMparamater_sql);
    //Name
    query->addBindValue(name);
    //防护舱参数
    query->addBindValue(pdat->fangHuCang.openLockTime);
    query->addBindValue(pdat->fangHuCang.OptBussinessTime);
    query->addBindValue(pdat->fangHuCang.timeOutRemind);
    query->addBindValue(pdat->fangHuCang.warnningDelayTime);
    query->addBindValue(pdat->fangHuCang.noManOpenLockTime);
    query->addBindValue(pdat->fangHuCang.fanRunTemperature);
    query->addBindValue(pdat->fangHuCang.signalModel);
    query->addBindValue(pdat->fangHuCang.peopleEquModel);
    query->addBindValue(pdat->fangHuCang.lockModel);
    query->addBindValue(pdat->fangHuCang.kongCangLockorNot);
    query->addBindValue(pdat->fangHuCang.fangQiewarnning);
    //写入照明参数
    query->addBindValue(pdat->zhaoMing.Model);
    query->addBindValue(pdat->zhaoMing.startHour);
    query->addBindValue(pdat->zhaoMing.startMinute);
    query->addBindValue(pdat->zhaoMing.endHour);
    query->addBindValue(pdat->zhaoMing.endMinute);
    query->addBindValue(pdat->zhaoMing.manEnable);
    //写入LED节目参数
    query->addBindValue(pdat->ledText[0]);
    query->addBindValue(pdat->ledText[1]);
    query->addBindValue(pdat->ledText[2]);
    //音乐参数
    query->addBindValue(pdat->soundVolume);
    query->addBindValue(pdat->soundMd5[0].toHex());
    query->addBindValue(pdat->soundMd5[1].toHex());
    query->addBindValue(pdat->soundMd5[2].toHex());
    query->addBindValue(pdat->soundMd5[3].toHex());
    query->addBindValue(pdat->soundMd5[4].toHex());
    query->addBindValue(pdat->soundMd5[5].toHex());
    query->addBindValue(pdat->soundMd5[6].toHex());
    query->addBindValue(pdat->soundMd5[7].toHex());
    query->addBindValue(pdat->soundMd5[8].toHex());
    //更新报警设置参数
    query->addBindValue(pdat->alarmPara.btnAlarmEnable);
    query->addBindValue(pdat->alarmPara.cutAlarmEnable);
    query->addBindValue(pdat->alarmPara.zhengDongAlarmEnable);
    query->addBindValue(pdat->alarmPara.yanWuAlarmEnable);
    query->addBindValue(pdat->alarmPara.boLiAlarmEnable);
    query->addBindValue(pdat->alarmPara.shuiQinAlarmEnable);
    query->addBindValue(pdat->alarmPara.tempAlarmEnable);
    //加钞间新增参数
    query->addBindValue(pdat->fangHuCang.userNum);
    query->addBindValue(pdat->fangHuCang.systemAlarmStatus);
    query->addBindValue(pdat->fangHuCang.setGuardDelayTime);
    query->addBindValue(pdat->fangHuCang.isMonitorOrNot);
    query->addBindValue(pdat->fangHuCang.inDoorModel);
    query->addBindValue(pdat->fangHuCang.outDoorModel);
    query->addBindValue(pdat->alarmPara.doorCiAlarmEnable);
    query->addBindValue(pdat->alarmPara.existManAlarmEnable);
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//插入数据库父表
void frmMain::Insert_TBelong_Sql(Equipment *temp)
{
    if(checkExist("TBelong","Mac",temp->equParaData.net.macAddr))
    {
        return;
    }
    //插入数据库父表
    query->prepare(insert_TBelong_sql);
    //Mac地址
    query->addBindValue(temp->equParaData.net.macAddr);
    //在树中父的名称
    query->addBindValue(temp->parentName);
    qDebug()<<"写入数据库："<<temp->parentName;
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//插入数据库模板表
void frmMain::Insert_TModule_Sql(paraModule *pModule)
{
    if(checkExist("TModule","Name",pModule->name))
    {
        return;
    }
    //插入数据库父表
    query->prepare(insert_TModule_sql);
    //名称
    query->addBindValue(pModule->name);
    //适用版本等级
    query->addBindValue(pModule->level);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//插入音乐到音乐库
void frmMain::Insert_TVoice_Sql(QString name,QString fileName,QString md5)
{
    if(checkExist("TVoice","Name",name))
    {
        return;
    }
    //插入音乐库
    query->prepare(insert_TVoice_sql);

    query->addBindValue(name);
    query->addBindValue(md5);
    query->addBindValue(fileName);
    query->addBindValue(2);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
void frmMain::InsertEquipment(Equipment *temp)
{
    //插入设备表
    Insert_TSlave_Sql(temp);
    //插入设备网络参数表
    Insert_TNetWork_Sql(temp);
    //插入所属区域表格
    Insert_TBelong_Sql(temp);
    //Insert_TParamater_Sql(temp);
}
void frmMain::slaveConnectSignal(Equipment * slave)
{
    slave->selectStatus=1;
    connect(slave,SIGNAL(sigDataUpdateToForm(Equipment*)),this,SLOT(updateParaDisplay(Equipment*)));
    connect(slave,SIGNAL(sigRefreshTime(paraData*)),this,SLOT(slotupdateSlaveTimeDisplay(paraData*)));
    connect(slave,SIGNAL(sigRefreshFangHuCangPara(paraData*)),this,SLOT(slotupdateSlaveFangHuCangParaDisplay(paraData *)));
    connect(slave,SIGNAL(sigRefreshStatus(paraData*)),this,SLOT(slotupdateSlaveStatusDisplay(paraData*)));
    connect(slave,SIGNAL(sigRefreshZhaoMingPara(paraData*)),this,SLOT(slotupdateSlaveZhaoMingParaDisplay(paraData*)));
    connect(slave,SIGNAL(sigRefreshSlavePara(paraData*)),this,SLOT(slotupdateSlaveParaDisplay(paraData*)));
    connect(slave,SIGNAL(sigRefreshLedPara(paraData*)),this,SLOT(slotupdateSlaveLedParaDisplay(paraData*)));
    connect(slave,SIGNAL(sigRefreshVoicePara(paraData*)),this,SLOT(slotupdateSlaveVoiceParaDisplay(paraData*)));
    connect(this,SIGNAL(sigRemoteCloseLock()),slave,SLOT(slotRemoteCloseLock()));
    connect(this,SIGNAL(sigEanbleOrDisableSlave()),slave,SLOT(slotEanbleOrDisableSlave()));
    connect(this,SIGNAL(sigReadSlaveMsg()),slave,SLOT(slotReadSlaveMsg()));
    connect(this,SIGNAL(sigOpenDoor(bool)),slave,SLOT(slotRemoteOpenDoor()));
    connect(this,SIGNAL(sigWriteNowTime(quint8,quint8,quint8,quint8,quint8,quint8,quint8)),slave,SLOT(slotSetSlaveTime(quint8,quint8,quint8,quint8,quint8,quint8,quint8)));
    connect(slave,SIGNAL(sigOpenDoorDone(quint8)),this,SLOT(slotShowOpenDoorMsg(quint8)));
    connect(slave,SIGNAL(sigCloseLockRet(quint8,quint8)),this,SLOT(slotShowCloseLockMsg(quint8,quint8)));
    //connect(frmEditSlavePara,SIGNAL(sigWriteFangHuCangPara(paraFangHuCang*)),slave,SLOT(slotSetWorkPara(paraFangHuCang*)));
    //connect(frmEditAlarm,SIGNAL(sigWriteAlarmPara(paraAlarm*)),slave,SLOT(slotSetAlarmPara(paraAlarm*)));
    //connect(frmEditZhaoMing,SIGNAL(sigWriteZhaoMingPara(paraZhaoMing*)),slave,SLOT(slotSetLineAndAdLinePara(paraZhaoMing*)));
    connect(slave,SIGNAL(sigSlaveReadedMsg(QList<SlaveMsg*>)),this,SLOT(slotInsertMsgToLog(QList<SlaveMsg *>)));
    connect(this,SIGNAL(sigSetLedText(quint8,QString)),slave,SLOT(slotSetLed(quint8,QString)));
    connect(this,SIGNAL(sigSetSlaveVolume(quint8)),slave,SLOT(slotSetHyVolume(quint8)));

    connect(this,SIGNAL(sigReadHySound()),slave,SLOT(slotReadHySound()));
    connect(this,SIGNAL(sigReadSyzSound()),slave,SLOT(slotReadSyzSound()));
    connect(this,SIGNAL(sigReadWhzSound()),slave,SLOT(slotReadWhzSound()));
    connect(this,SIGNAL(sigReadTimeoutTiShiSound()),slave,SLOT(slotReadTimeoutTiShiSound()));
    connect(this,SIGNAL(sigReadTimeoutWarnSound()),slave,SLOT(slotReadTimeoutWarnSound()));
    connect(this,SIGNAL(sigReadOutDoorSound()),slave,SLOT(slotReadOutDoorSound()));
    connect(this,SIGNAL(sigReadOpenDoorSound()),slave,SLOT(slotReadOpenDoorSound()));
    connect(this,SIGNAL(sigReadFailLockSound()),slave,SLOT(slotReadFailLockSound()));
    connect(this,SIGNAL(sigReadLaDoorSound()),slave,SLOT(slotReadLaDoorSound()));

    connect(slave,SIGNAL(sigSaveSound(paraData*)),this,SLOT(slotShowSoundSaveWindow(paraData*)));
    connect(slave,SIGNAL(sigSoundSize(quint64)),this,SLOT(slotShowReadSoundProcess(quint64)));
    connect(slave,SIGNAL(sigUseModule(quint64)),this,SLOT(slotShowUseModuleProcess(quint64)));
    connect(slave,SIGNAL(sigReadSoundSize(quint64)),frmReadSoundProcess,SLOT(setProcessCurrentValue(quint64)));
    connect(slave,SIGNAL(sigUseModuleSize(quint64)),frmReadSoundProcess,SLOT(setPBarMainCurrentValue(quint64)));
    connect(slave,SIGNAL(closeWindows()),frmReadSoundProcess,SLOT(close()));
    connect(slave,SIGNAL(sigSetWindowsTitle(QString)),frmReadSoundProcess,SLOT(slotSetWindowsText(QString)));
    //connect(this,SIGNAL(sigChangeSoundIdAndMd5(quint8,QString)),slave,SLOT(slotSetMd5(quint8,QString)));
    //connect(this,SIGNAL(sigChangeSoundIdAndName(quint8,QString)),slave,SLOT(slotSetSound(quint8,QString)));
    connect(this,SIGNAL(sigChangeSound(quint8,QString,QString)),slave,SLOT(slotChangeSlaveSound(quint8,QString,QString)));
    connect(this,SIGNAL(sigPlaySlaveSound(quint8)),slave,SLOT(slotPlaySound(quint8)));
    connect(this,SIGNAL(sigUseModule(paraData*)),slave,SLOT(slotUseModule(paraData*)));
    connect(slave,SIGNAL(sigChangeSoundFromModule(paraData*,quint8)),this,SLOT(slotUseModuleSound(paraData*,quint8)));

    connect(slave,SIGNAL(sigRefreshAlarmPara(paraData*)),this,SLOT(slotupdateSlaveAlarmParaDisplay(paraData*)));
    connect(frmAlarm,SIGNAL(sigClearAlarm()),slave,SLOT(slotClearAlarm()));

    connect(slave,SIGNAL(sigInsertFangHuCangLog(QList<FangHuCangLogItem*>)),this,SLOT(slotShowFangHuCangLog(QList<FangHuCangLogItem*>)));

}
void frmMain::slaveDisConnectSignal(Equipment * slave)
{
    slave->selectStatus=0;
    disconnect(slave,SIGNAL(sigDataUpdateToForm(Equipment*)),this,SLOT(updateParaDisplay(Equipment*)));
    disconnect(this,SIGNAL(sigOpenDoor(bool)),slave,SLOT(slotRemoteOpenDoor()));
    disconnect(this,SIGNAL(sigRemoteCloseLock()),slave,SLOT(slotRemoteCloseLock()));
    disconnect(this,SIGNAL(sigEanbleOrDisableSlave()),slave,SLOT(slotEanbleOrDisableSlave()));
    disconnect(this,SIGNAL(sigReadSlaveMsg()),slave,SLOT(slotReadSlaveMsg()));
    disconnect(this,SIGNAL(sigWriteNowTime(quint8,quint8,quint8,quint8,quint8,quint8,quint8)),slave,SLOT(slotSetSlaveTime(quint8,quint8,quint8,quint8,quint8,quint8,quint8)));
    disconnect(slave,SIGNAL(sigOpenDoorDone(quint8)),this,SLOT(slotShowOpenDoorMsg(quint8)));
    disconnect(slave,SIGNAL(sigCloseLockRet(quint8,quint8)),this,SLOT(slotShowCloseLockMsg(quint8,quint8)));
    //disconnect(frmEditSlavePara,SIGNAL(sigWriteFangHuCangPara(paraFangHuCang*)),slave,SLOT(slotSetWorkPara(paraFangHuCang*)));
    //disconnect(frmEditZhaoMing,SIGNAL(sigWriteZhaoMingPara(paraZhaoMing*)),slave,SLOT(slotSetLineAndAdLinePara(paraZhaoMing*)));
    disconnect(slave,SIGNAL(sigRefreshTime(paraData*)),this,SLOT(slotupdateSlaveTimeDisplay(paraData*)));
    disconnect(slave,SIGNAL(sigRefreshFangHuCangPara(paraData*)),this,SLOT(slotupdateSlaveFangHuCangParaDisplay(paraData *)));
    disconnect(slave,SIGNAL(sigRefreshStatus(paraData*)),this,SLOT(slotupdateSlaveStatusDisplay(paraData*)));
    disconnect(slave,SIGNAL(sigRefreshZhaoMingPara(paraData*)),this,SLOT(slotupdateSlaveZhaoMingParaDisplay(paraData*)));
    disconnect(slave,SIGNAL(sigRefreshSlavePara(paraData*)),this,SLOT(slotupdateSlaveParaDisplay(paraData*)));
    disconnect(slave,SIGNAL(sigRefreshLedPara(paraData*)),this,SLOT(slotupdateSlaveLedParaDisplay(paraData*)));
    disconnect(slave,SIGNAL(sigRefreshVoicePara(paraData*)),this,SLOT(slotupdateSlaveVoiceParaDisplay(paraData*)));
    disconnect(slave,SIGNAL(sigSlaveReadedMsg(QList<SlaveMsg*>)),this,SLOT(slotInsertMsgToLog(QList<SlaveMsg *>)));
    disconnect(this,SIGNAL(sigSetLedText(quint8,QString)),slave,SLOT(slotSetLed(quint8,QString)));
    disconnect(this,SIGNAL(sigSetSlaveVolume(quint8)),slave,SLOT(slotSetHyVolume(quint8)));

    disconnect(this,SIGNAL(sigReadHySound()),slave,SLOT(slotReadHySound()));
    disconnect(this,SIGNAL(sigReadSyzSound()),slave,SLOT(slotReadSyzSound()));
    disconnect(this,SIGNAL(sigReadWhzSound()),slave,SLOT(slotReadWhzSound()));
    disconnect(this,SIGNAL(sigReadTimeoutTiShiSound()),slave,SLOT(slotReadTimeoutTiShiSound()));
    disconnect(this,SIGNAL(sigReadTimeoutWarnSound()),slave,SLOT(slotReadTimeoutWarnSound()));
    disconnect(this,SIGNAL(sigReadOutDoorSound()),slave,SLOT(slotReadOutDoorSound()));
    disconnect(this,SIGNAL(sigReadOpenDoorSound()),slave,SLOT(slotReadOpenDoorSound()));
    disconnect(this,SIGNAL(sigReadFailLockSound()),slave,SLOT(slotReadFailLockSound()));
    disconnect(this,SIGNAL(sigReadLaDoorSound()),slave,SLOT(slotReadLaDoorSound()));

    disconnect(slave,SIGNAL(sigSaveSound(paraData*)),this,SLOT(slotShowSoundSaveWindow(paraData*)));
    disconnect(slave,SIGNAL(sigSoundSize(quint64)),this,SLOT(slotShowReadSoundProcess(quint64)));
    disconnect(slave,SIGNAL(sigUseModule(quint64)),this,SLOT(slotShowUseModuleProcess(quint64)));
    disconnect(slave,SIGNAL(sigReadSoundSize(quint64)),frmReadSoundProcess,SLOT(setProcessCurrentValue(quint64)));
    disconnect(slave,SIGNAL(sigUseModuleSize(quint64)),frmReadSoundProcess,SLOT(setPBarMainCurrentValue(quint64)));
    disconnect(slave,SIGNAL(closeWindows()),frmReadSoundProcess,SLOT(close()));
    disconnect(slave,SIGNAL(sigSetWindowsTitle(QString)),frmReadSoundProcess,SLOT(slotSetWindowsText(QString)));
    //disconnect(this,SIGNAL(sigChangeSoundIdAndMd5(quint8,QString)),slave,SLOT(slotSetMd5(quint8,QString)));
    //disconnect(this,SIGNAL(sigChangeSoundIdAndName(quint8,QString)),slave,SLOT(slotSetSound(quint8,QString)));
    disconnect(this,SIGNAL(sigChangeSound(quint8,QString,QString)),slave,SLOT(slotChangeSlaveSound(quint8,QString,QString)));
    disconnect(this,SIGNAL(sigPlaySlaveSound(quint8)),slave,SLOT(slotPlaySound(quint8)));
    disconnect(this,SIGNAL(sigUseModule(paraData*)),slave,SLOT(slotUseModule(paraData*)));
    disconnect(slave,SIGNAL(sigChangeSoundFromModule(paraData*,quint8)),this,SLOT(slotUseModuleSound(paraData*,quint8)));
    //disconnect(frmEditAlarm,SIGNAL(sigWriteAlarmPara(paraAlarm*)),slave,SLOT(slotSetAlarmPara(paraAlarm*)));
    disconnect(slave,SIGNAL(sigRefreshAlarmPara(paraData*)),this,SLOT(slotupdateSlaveAlarmParaDisplay(paraData*)));
    disconnect(frmAlarm,SIGNAL(sigClearAlarm()),slave,SLOT(slotClearAlarm()));
    disconnect(slave,SIGNAL(sigInsertFangHuCangLog(QList<FangHuCangLogItem*>)),this,SLOT(slotShowFangHuCangLog(QList<FangHuCangLogItem*>)));

}
void frmMain::slaveChangeConnectToForm(Equipment* tempSlave,QList<TreeViewItem*> *tempList)
{
    //重新关联信号
    for(int i=0;i<tempList->count();i++)
    {
        Equipment* tempEqu = tempList->at(i)->GetSlave();
        if(tempEqu)
        {
            if(tempEqu->selectStatus==1)
            {
                tempEqu->selectStatus=0;
                slaveDisConnectSignal(tempEqu);
                break;
            }
        }

    }

    slaveConnectSignal(tempSlave);
    updateParaDisplay(tempSlave);
    tempSlave->slotReadSlaveTime();
    ui->tabWidgetSlavePara->setCurrentIndex(0);
}
void frmMain::refreshListView(QModelIndex index)
{
    //显示设备列表
    QList<TreeViewItem*> slaveList = model->GetChildsList(index);
    slaveListModel->setData(slaveList);
    ui->listView->setModel(slaveListModel);
    ui->listView->setIconSize(QSize(16,16));
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setGridSize(QSize(80,40));

}
//鼠标点击treeView的条目时触发
void frmMain::slotchangeSlaveDisplay(QModelIndex index)
{

    Equipment* tempSlave = model->GetSlave(index);
    QList<TreeViewItem*> *tempList = model->GetData();
    QModelIndex par = model->parent(index);

    //更新页面

    if(tempSlave)
    {
        slaveChangeConnectToForm(tempSlave,tempList);
        changeStockedPageDisplay(1);
        if(parentIndex != par)
        {
            parentIndex = par;
            //列表更新显示设备
            refreshListView(par);
        }
    }
    else
    {
        //列表更新显示设备
        //refreshListView(index);
        changeStockedPageDisplay(0);
    }

    //ui->listView->clearSelection();
}
//在treeView上的右键
void frmMain::slotTreeViewCustomContextMenuRequested(const QPoint &pos)
{
    static QAction *expandAllAction;
    static QAction *unExpandAllAction;
    static QAction *deleteItemAction;
    static QAction *moveToParentAction;
    static QAction *editAction;
    static QAction *addParentInTreeAction;
    static QAction *editAreaAllSlaveZhaoMingPara;
    static QAction *editAreaAllSlaveWorkPara;
    if (treeViewContextMenu == NULL)
    {
        treeViewContextMenu = new QMenu(this);

        expandAllAction = new QAction(treeViewContextMenu);
        unExpandAllAction = new QAction(treeViewContextMenu);
        deleteItemAction = new QAction(treeViewContextMenu);
        moveToParentAction = new QAction(treeViewContextMenu);
        editAction = new QAction(treeViewContextMenu);
        addParentInTreeAction = new QAction(treeViewContextMenu);
        editAreaAllSlaveZhaoMingPara = new QAction(treeViewContextMenu);
        editAreaAllSlaveWorkPara = new QAction(treeViewContextMenu);

        expandAllAction->setText(tr("全部展开"));
        unExpandAllAction->setText(tr("全部折叠"));
        deleteItemAction->setText(tr("删除设备"));
        moveToParentAction->setText(tr("移动到区域"));
        editAction->setText(tr("编辑名称"));
        addParentInTreeAction->setText(tr("添加区域"));
        editAreaAllSlaveZhaoMingPara->setText("批量修改本区域门禁照明参数");
        editAreaAllSlaveWorkPara->setText("批量修改本区域门禁工作参数");

        treeViewContextMenu->addAction(expandAllAction);
        treeViewContextMenu->addAction(unExpandAllAction);
        treeViewContextMenu->addAction(deleteItemAction);
        treeViewContextMenu->addAction(moveToParentAction);
        treeViewContextMenu->addAction(editAction);
        treeViewContextMenu->addAction(addParentInTreeAction);

        connect(expandAllAction, SIGNAL(triggered()),ui->treeView, SLOT(expandAll()));
        connect(unExpandAllAction, SIGNAL(triggered()),ui->treeView, SLOT(collapseAll()));
        connect(deleteItemAction, SIGNAL(triggered()),this, SLOT(slotdeleteItem()));
        connect(editAction,SIGNAL(triggered(bool)),this,SLOT(slotEditItemMessageBox()));
        connect(addParentInTreeAction,SIGNAL(triggered(bool)),this,SLOT(slotAddNewParentInTree()));
        connect(moveToParentAction,SIGNAL(triggered(bool)),this,SLOT(slotmoveToArea()));
        connect(editAreaAllSlaveZhaoMingPara,SIGNAL(triggered(bool)),this,SLOT(slotEditAreaSlaveZhaoMingPara()));
        connect(editAreaAllSlaveWorkPara,SIGNAL(triggered(bool)),this,SLOT(slotEditAreaSlaveWorkPara()));
    }
    TreeItem *item= model->GetTreeItem(ui->treeView->currentIndex());
    if(item)
    {
        if(item->data(2)==-1)
        {
            editAction->setVisible(true);
            addParentInTreeAction->setVisible(true);
            moveToParentAction->setVisible(false);
            deleteItemAction->setText(tr("删除区域"));
            treeViewContextMenu->addAction(editAreaAllSlaveZhaoMingPara);
            treeViewContextMenu->addAction(editAreaAllSlaveWorkPara);
        }
        else
        {
            moveToParentAction->setVisible(true);
            editAction->setVisible(false);
            addParentInTreeAction->setVisible(false);
            deleteItemAction->setText(tr("删除设备"));
            treeViewContextMenu->removeAction(editAreaAllSlaveZhaoMingPara);
            treeViewContextMenu->removeAction(editAreaAllSlaveWorkPara);
        }
        treeViewContextMenu->exec(cursor().pos());
    }
}
//鼠标点击listView的条目时触发
void frmMain::slotchangeSlaveDisplayFromListView(QModelIndex index)
{

    Equipment* tempSlave = slaveListModel->GetSlave(index);
    QList<TreeViewItem*> *tempList = model->GetData();
    //更新页面

    if(tempSlave)
    {
        changeStockedPageDisplay(1);
        slaveChangeConnectToForm(tempSlave,tempList);
    }
    else
    {
        changeStockedPageDisplay(0);
    }
    //ui->treeView->clearSelection();
}
void frmMain::slotSelectParentIndex(QString name)
{
    selectedParentName=name;
}
QList<paraData *> frmMain::removeRegisteredSlave(QList<paraData*> list)
{
    QList<paraData*> retList;

    retList.clear();
    for(int i=0;i<list.count();i++)
    {
        if(!checkExist("TSlave","Mac",list.at(i)->net.macAddr))
        {
            retList.append(list.at(i));
        }
    }
    return retList;
}

/*判断是否在表table中存在一条item的值为value的记录
 *table:项所在的表格
 * item:项
 * value:值
 * 返回：1－存在 0－不存在
 */
bool frmMain::checkExist(QString table,QString item,QString value)
{
    QSqlQuery* querytemp = new QSqlQuery(database);
    if(!querytemp->exec("SELECT "+item+" FROM "+table+" WHERE "+item+"='"+value+"'"))
    {
        QLOG_ERROR() <<querytemp->lastError();
    }
    else
    {
        if(querytemp->next())
        {
            return true;
        }
    }
    delete querytemp;
    return false;
}
/*判断是否在表table中存在一条item的值为value的记录
 *table:项所在的表格
 * item:项
 * value:值
 * 返回：1－存在 0－不存在
 */
bool frmMain::checkExist(QString table,QString item,int value)
{
    QSqlQuery* querytemp = new QSqlQuery(database);
    if(!querytemp->exec("SELECT "+item+" FROM "+table+" WHERE "+item+"="+QString::number(value)))
    {
        QLOG_ERROR() <<querytemp->lastError();
    }
    else
    {
        if(querytemp->next())
        {
            delete querytemp;
            return true;
        }
    }
    delete querytemp;
    return false;
}
//插入数据库树形父名称表
void frmMain::Insert_TZone_Sql(QString name,int areaIndex)
{
    if(checkExist("TZone","ZName",name))
    {
        return;
    }
    QSqlQuery* querytemp = new QSqlQuery(database);
    //插入数据库树形父名称表
    querytemp->prepare(insert_TZone_sql);
    //父名称
    querytemp->addBindValue(name);
    //在树中的index
    querytemp->addBindValue(areaIndex);

    if(!querytemp->exec())
    {
        QLOG_ERROR() <<querytemp->lastError();
    }
    delete querytemp;
    getParentTreeModel();
}
void frmMain::getParentTreeModel()
{
    treeViewParentTbModel->setTable("TZone");
    treeViewParentTbModel->select();
    treeViewParentTbModel->setHeaderData(0,Qt::Horizontal,tr("区域名称"));
    emit sigSendParentTbModel(treeViewParentTbModel);
}
void frmMain::slotParentInTreeDataChanged(bool changed)
{
    //区域发生变化后重新读取数据
    bool exist = true;
    //InitForm();
    getParentTreeModel();
    if(1)
    {
        for(int i=0;i<viewlist.count();i++)
        {
            qDebug()<<"rows:"<<treeViewParentTbModel->rowCount();
            for(int j=0;j<treeViewParentTbModel->rowCount();j++)
            {
                QModelIndex index = treeViewParentTbModel->index(j,0);
                qDebug()<<treeViewParentTbModel->data(index).toString();

                qDebug()<<treeViewParentTbModel->data(index);
                if((viewlist.at(i)->parentIndexInTree == -1)&&(viewlist.at(i)->text == treeViewParentTbModel->data(index).toString()))
                {
                    qDebug()<<treeViewParentTbModel->data(index).toString()<<"exist";
                    exist = false;
                    break;
                }
            }
            if(exist&&(viewlist.at(i)->parentIndexInTree == -1))
            {
                qDebug()<<viewlist.at(i)->text<<"removed";
                viewlist.removeAt(i);
                i--;

            }
            exist = true;
        }
    }
    updateSlaveListDisplay();
}
void frmMain::slotTreeParentChanged(QModelIndex index1,QModelIndex index2)
{
    //qDebug()<<treeViewParentTbModel->data(index1).toString();
}
QList<TreeViewItem*> frmMain::getSortedList(QList<TreeViewItem*> list)
{
    //先按parentIndex从小到大排序
    for(int i = 0; i<list.length(); i++)
    {
        for (int j = i+1; j < list.length(); j++)
        {
            //排序条件:parentIndex从小到大
            if(list.at(i)->parentIndexInTree > list.at(j)->parentIndexInTree)
            {
                list.swap(i,j);
            }
        }
    }
    //先按parentIndex相等时 index从小到大排序
    for(int i = 0; i<list.length(); i++)
    {
        for (int j = i+1; j < list.length(); j++)
        {
            //排序条件:parentIndex相等时 index从小到大排序
            if(list.at(i)->parentIndexInTree ==list.at(j)->parentIndexInTree)
            {
                if(list.at(i)->index > list.at(j)->index)
                {
                    list.swap(i,j);
                }
            }
        }
    }
    return list;
}
void frmMain::slotEditItemMessageBox()
{
    TreeItem *item= model->GetTreeItem(ui->treeView->currentIndex());
    for(int i=0;i<viewlist.count();i++)
    {
        if(viewlist.at(i)->mac == item->data(0))
        {
            if(viewlist.at(i)->parentIndexInTree !=-1)
                return;
            editedName="";
            frmOperateItemBox->SetMessage(viewlist.at(i)->text, 1);
            frmOperateItemBox->exec();

            //修改viewlist的名称后刷新
            if(editedName!="")
            {
                viewlist.at(i)->text = editedName;
                //更新树结构并显示
                updateSlaveListDisplay();
                //写入数据库
                Update_TZone_name_sql(viewlist.at(i)->text,viewlist.at(i)->index);
            }

            break;
        }
    }
    //重新获取区域列表
    getParentTreeModel();
}

void frmMain::slotdeleteItem()
{
    int parentIdx=0;
    bool refresh=false;
    QList<TreeViewItem*> childsList;

    TreeItem *item= model->GetTreeItem(ui->treeView->currentIndex());

    for(int i=0;i<viewlist.count();i++)
    {
        if(viewlist.at(i)->mac == item->data(0))
        {
            if(viewlist.at(i)->parentIndexInTree != -1)
            {
                //记录删除的项的index即在树中的位置
                parentIdx=viewlist.at(i)->parentIndexInTree;
                DeleteSlave(viewlist.at(i)->mac);
                //断开连接
                Equipment *temp = viewlist.at(i)->GetSlave();
                slaveDisConnectSignal(temp);
                //解除关联信号
                disconnectToUpdateSlaveToDB(temp);
                //释放内存
                viewlist.at(i)->GetSlave()->deleteLater();
                viewlist.at(i)->deleteLater();
                viewlist.removeAt(i);
                temp->deleteLater();

                refresh = true;
            }
            else
            {
                if(item->GetChildItems().count() == 0)
                {
                    parentIdx=viewlist.at(i)->parentIndexInTree;
                    DeleteParentInTree(viewlist.at(i)->text);
                    viewlist.removeAt(i);
                    refresh = true;
                }
                else
                {
                    myHelper::ShowMessageBoxError("区域存在子设备，请将设备移除后再删除区域！");
                }
            }
            //更新树结构并显示
            updateSlaveListDisplay();
            break;//停止循环
        }
    }
    //如果删除的是设备节点则需要刷新index
    if(refresh)
    {
        //刷新数据库设备index
        for(int i=0;i<viewlist.count();i++)
        {
            if(viewlist.at(i)->parentIndexInTree == parentIdx)
            {
                childsList.append(viewlist.at(i));
            }
        }
        for(int i=0;i<childsList.count();i++)
        {
            //如果是父节点
            if(parentIdx == -1)
            {
                //更新viewlist数据
                childsList.at(i)->index = i;
                //更新数据库
                Update_TZone_index_sql(childsList.at(i)->text,i);
            }else//如果是子节点
            {
                //更新viewlist数据
                childsList.at(i)->index = i;
                //更新数据库
                Update_TSlave_index_sql(childsList.at(i)->mac,i);
            }
        }
    }
    //重新获取区域列表
    getParentTreeModel();
    //刷新list列表
    refreshListView(ui->treeView->currentIndex());
    updateParaDisplay(NULL);

}
void frmMain::DeleteSlave(QString mac)
{
    //删掉设备表里的数据
    delete_TSlave_Sql(mac);
    //删掉设备参数
    delete_TParamater_Sql(mac);
    //删掉设备网络参数
    delete_TNetWork_Sql(mac);
    //删掉TBelong表里的数据
    delete_TBelong_Sql(mac);
}
void frmMain::DeleteParentInTree(QString name)
{
    delete_TZone_Sql(name);
}
//删除数据库设备
void frmMain::delete_TSlave_Sql(QString mac)
{
    if(!checkExist("TSlave","Mac",mac))
    {
        return;
    }
    //删除项目
    query->prepare(delete_TSlave_sql);
    //mac地址
    query->addBindValue(mac);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//删除数据库设备网络参数
void frmMain::delete_TNetWork_Sql(QString mac)
{
    if(!checkExist("SlaveNetWork","Mac",mac))
    {
        return;
    }
    //删除项目
    query->prepare(delete_TNetWork_sql);
    //mac地址
    query->addBindValue(mac);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//删除设备参数
void frmMain::delete_TParamater_Sql(QString mac)
{
    if(!checkExist("TParamater","Mac",mac))
    {
        return;
    }
    //删除项目
    query->prepare(delete_TParamater_sql);
    //mac地址
    query->addBindValue(mac);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//删除数据库设备
void frmMain::delete_TBelong_Sql(QString mac)
{
    if(!checkExist("TBelong","Mac",mac))
    {
        return;
    }
    //删除项目
    query->prepare(delete_TBelong_sql);
    //mac地址
    query->addBindValue(mac);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//删除区域
void frmMain::delete_TZone_Sql(QString name)
{
    if(!checkExist("TZone","ZName",name))
    {
        return;
    }
    //删除项目
    query->prepare(delete_TZone_sql);
    //mac地址
    query->addBindValue(name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//删除声音
void frmMain::delete_TVoice_Sql(QString name)
{
    QString fileName,md5;
    if(!checkExist("TVoice","Name",name))
    {
        return;
    }
    //删除文件
    Select_TVoice_FileName_Md5(name,fileName,md5);
    QFile::remove(sFilePath+fileName);
    //删除数据库
    query->prepare(delete_TVoice_sql);
    //mac地址
    query->addBindValue(name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }

}
//删除模板
void frmMain::delete_TModule_Sql(QString name)
{
    if(!checkExist("TModule","Name",name))
    {
        return;
    }

    //删除数据库
    query->prepare(delete_TModule_sql);
    //mac地址
    query->addBindValue(name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }

}
//删除模板参数
void frmMain::delete_TMparamater_Sql(QString name)
{
    if(!checkExist("TMparamater","Name",name))
    {
        return;
    }

    //删除数据库
    query->prepare(delete_TMparamater_sql);
    //mac地址
    query->addBindValue(name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }

}
//更新数据库TBelong
void frmMain::Update_TBelong_ZName_sql(QString mac,QString name)
{
    if(!checkExist("TBelong","Mac",mac))
    {
        return;
    }
    //更新项目
    query->prepare(update_TBelong_ZName_sql);
    query->bindValue(":name", name);
    query->bindValue(":Mac", mac);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}

//更新数据库设备indexTnTree
void frmMain::Update_TSlave_index_sql(QString mac,int index)
{
    if(!checkExist("TSlave","Mac",mac))
    {
        return;
    }
    //更新项目
    query->prepare(update_TSlave_index_sql);
    query->bindValue(":index", index);
    query->bindValue(":Mac", mac);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
void frmMain::slotUpdateTSlave(Equipment *slave)
{
    Update_TSlave_sql(slave);
}
//更新数据库设备
void frmMain::Update_TSlave_sql(Equipment *slave)
{

    if(!checkExist("TSlave","Mac",slave->equParaData.net.macAddr))
    {
        return;
    }
    QSqlQuery *querytemp = new QSqlQuery(database);
    //更新项目
    querytemp->prepare(update_TSlave_sql);
    querytemp->bindValue(":type",slave->equParaData.slaveType);
    querytemp->bindValue(":version", slave->equParaData.version);
    querytemp->bindValue(":name", slave->equParaData.name);
    querytemp->bindValue(":ip", slave->equParaData.net.ip);

    querytemp->bindValue(":Mac", slave->equParaData.net.macAddr);
    if(!querytemp->exec())
    {
        QLOG_ERROR() <<querytemp->lastError();
    }
    delete querytemp;
}

//
void frmMain::Update_TModule_sql(QString name)
{
    if(!checkExist("TModule","Name",name))
    {
        return;
    }
    //更新项目
    query->prepare(update_TModule_sql);
    query->bindValue(":name", name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//更新区域indexTnTree
void frmMain::Update_TZone_index_sql(QString name,int index)
{
    if(!checkExist("TZone","ZName",name))
    {
        return;
    }
    //更新项目
    query->prepare(update_TZone_index_sql);
    query->bindValue(":index", index);
    query->bindValue(":name", name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//更新区域Name
void frmMain::Update_TZone_name_sql(QString name,int index)
{
    if(!checkExist("TZone","IndexInTree",index))
    {
        return;
    }
    //更新项目
    query->prepare(update_TZone_name_sql);
    query->bindValue(":index", index);
    query->bindValue(":name", name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
//更新区域Name
void frmMain::Update_TVoice_Sql(QString name,QString md5,QString fileName)
{
    if(!checkExist("TVoice","Name",name))
    {
        return;
    }
    //更新项目
    query->prepare(update_TVoice_sql);
    query->bindValue(":md5", md5);
    query->bindValue(":fileName", fileName);
    query->bindValue(":name", name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
void frmMain::sloteditedName(QString name)
{
    editedName = name;
}
void frmMain::slotAddNewParentInTree()
{
    int parent=0;
    editedName="";
    frmOperateItemBox->SetMessage("新建区域", 0);
    frmOperateItemBox->exec();
    //修改viewlist的名称后刷新
    if(editedName!="")
    {
        //获取区域的个数
        if(!query->exec("SELECT * FROM TZone"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                parent++;
            }
        }
        TreeViewItem * newParent = new TreeViewItem();
        newParent->parentIndexInTree = -1;
        newParent->index = parent++;
        newParent->mac = newParent->index;
        newParent->text = editedName;

        viewlist.append(newParent);
        //更新树结构并显示
        updateSlaveListDisplay();
        //写入数据库
        Insert_TZone_Sql(newParent->text,newParent->index);
        //重新获取区域列表
        getParentTreeModel();

        emit sigSendParentTbModel(treeViewParentTbModel);
    }
}
void frmMain::slotmoveToArea()
{
    int preParentIdx=0;//移动前项的父的index
    bool refresh = false;//是否刷新
    selectedParentName="";                                  //清除上次选择的区域的名称
    //获取需要移动的项
    TreeItem *item= model->GetTreeItem(ui->treeView->currentIndex());
    //获取所选项的父 后面会对此父的子刷新index
    preParentIdx = item->data(2).toInt();

    emit sigSendParentTbModel(treeViewParentTbModel);       //发送区域信号
    frmSelectParentInTree->exec();                          //运行选择窗口
    if(frmSelectParentInTree->result()==QDialog::Rejected)
    {
        return;//如果点击取消或关闭则返回
    }
    if(selectedParentName!="")
    {
        //如果选择的区域的名称为空则不执行
        //查找选择的区域的index
        int parentIndex=0;
        int childs=0;
        if(!query->exec("SELECT IndexInTree FROM TZone WHERE ZName='"+selectedParentName+"'"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                parentIndex = query->value(0).toInt();
            }
        }
        //如果选择的是原来的父则直接返回
        if(preParentIdx == parentIndex)
            return;
        //获取所选区域的child的个数
        if(!query->exec("SELECT Mac FROM TBelong WHERE ZName='"+selectedParentName+"'"))
        {
            QLOG_ERROR() <<query->lastError();
        }
        else
        {
            while(query->next())
            {
                childs++;
            }
        }
        //遍历找到选择的项对应的底层数据
        for(int i=0;i<viewlist.count();i++)
        {
            if(viewlist.at(i)->mac == item->data(0))
            {
                if(viewlist.at(i)->parentIndexInTree != -1)
                {
                    viewlist.at(i)->parentIndexInTree = parentIndex;
                    viewlist.at(i)->index = childs++;
                    //写入数据库
                    Update_TSlave_index_sql(viewlist.at(i)->mac,viewlist.at(i)->index);
                    Update_TBelong_ZName_sql(viewlist.at(i)->mac,selectedParentName);
                    refresh = true;
                    //更新树结构并显示
                    updateSlaveListDisplay();
                    break;//停止循环
                }

            }
        }
        QList<TreeViewItem *> childsList;
        childsList.clear();
        if(refresh)
        {
            //刷新数据库设备index
            for(int i=0;i<viewlist.count();i++)
            {
                if(viewlist.at(i)->parentIndexInTree == preParentIdx)
                {
                    childsList.append(viewlist.at(i));
                }
            }
            for(int i=0;i<childsList.count();i++)
            {
                //如果是父节点
                if(preParentIdx != -1)
                {
                    //更新viewlist数据
                    childsList.at(i)->index = i;
                    //更新数据库
                    Update_TSlave_index_sql(childsList.at(i)->mac,i);
                }
            }
        }
        //添加日志记录
        InsertOneSystemLogAndShow("移除区域"+selectedParentName);
    }
}
void frmMain::Update_TMparamater_Sql(QString name,paraData *pdat)
{
    if(!checkExist("TModule","Name",name))
    {
        return;
    }
    paraFangHuCang fhcPara = pdat->fangHuCang;
    paraZhaoMing zmPara = pdat->zhaoMing;
    paraAlarm alarm = pdat->alarmPara;
    //更新项目
    query->prepare(update_TMparamater_sql);
    //更新防护舱参数
    query->bindValue(":OpenLockTime", fhcPara.openLockTime);
    query->bindValue(":OperationTime", fhcPara.OptBussinessTime);
    query->bindValue(":RemindTimeout", fhcPara.timeOutRemind);
    query->bindValue(":Interval", fhcPara.warnningDelayTime);
    query->bindValue(":NoManOpenTime", fhcPara.noManOpenLockTime);
    query->bindValue(":OpenFanTemp", fhcPara.fanRunTemperature);
    query->bindValue(":HumanDetectingInputType", fhcPara.signalModel);
    query->bindValue(":HumanDetectingType", fhcPara.peopleEquModel);
    query->bindValue(":LockType", fhcPara.lockModel);
    query->bindValue(":LockWhileIdle", fhcPara.kongCangLockorNot);
    query->bindValue(":FangQiewarning", fhcPara.fangQiewarnning);
    //更新照明参数
    query->bindValue(":Mode", zmPara.Model);
    query->bindValue(":startHour", zmPara.startHour);
    query->bindValue(":startMinute", zmPara.startMinute);
    query->bindValue(":stopHour", zmPara.endHour);
    query->bindValue(":stopMinute", zmPara.endMinute);
    query->bindValue(":manEnable", zmPara.manEnable);
    //更新LED节目
    query->bindValue(":lpmWelcome", pdat->ledText[0]);
    query->bindValue(":lpmUsing", pdat->ledText[1]);
    query->bindValue(":lpmMaintaining", pdat->ledText[2]);
    //更新音乐参数
    query->bindValue(":volume", pdat->soundVolume);
    query->bindValue(":welcome", pdat->getSoundMd5(0));
    query->bindValue(":using", pdat->getSoundMd5(1));
    query->bindValue(":maintaining", pdat->getSoundMd5(2));
    query->bindValue(":reminding", pdat->getSoundMd5(3));
    query->bindValue(":timeout",pdat->getSoundMd5(4));
    query->bindValue(":goodbye",pdat->getSoundMd5(5));
    query->bindValue(":doorOpen",pdat->getSoundMd5(6));
    query->bindValue(":notLocked",pdat->getSoundMd5(7));
    query->bindValue(":slideDoor",pdat->getSoundMd5(8));
    //更新报警设置参数
    query->bindValue(":btnAlarmEnable", alarm.btnAlarmEnable);
    query->bindValue(":cutAlarmEnable", alarm.cutAlarmEnable);
    query->bindValue(":zhengDongAlarmEnable",alarm.zhengDongAlarmEnable);
    query->bindValue(":yanWuAlarmEnable",alarm.yanWuAlarmEnable);
    query->bindValue(":boLiAlarmEnable",alarm.boLiAlarmEnable);
    query->bindValue(":shuiQinAlarmEnable",alarm.shuiQinAlarmEnable);
    query->bindValue(":tempAlarmEnable",alarm.tempAlarmEnable);
    //加钞间参数
    query->bindValue(":userNum", fhcPara.userNum);
    query->bindValue(":systemAlarmStatus", fhcPara.systemAlarmStatus);
    query->bindValue(":setGuardDelayTime", fhcPara.setGuardDelayTime);
    query->bindValue(":isMonitorOrNot", fhcPara.isMonitorOrNot);
    query->bindValue(":inDoorModel", fhcPara.inDoorModel);
    query->bindValue(":outDoorModel", fhcPara.outDoorModel);

    query->bindValue(":doorCiAlarmEnable",alarm.doorCiAlarmEnable);
    query->bindValue(":existManAlarmEnable",alarm.existManAlarmEnable);

    query->bindValue(":name", name);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
void frmMain::Update_TParamater_Sql(Equipment *slave)
{
    QString mac = slave->equParaData.net.macAddr;
    if(!checkExist("TParamater","Mac",mac))
    {
        return;
    }
    paraData * temp = slave->GetEquAll();
    paraFangHuCang fhcPara = temp->fangHuCang;
    paraZhaoMing zmPara = temp->zhaoMing;
    paraAlarm alarm = temp->alarmPara;
    //更新项目
    query->prepare(update_TPara_sql);
    //更新防护舱参数
    query->bindValue(":OpenLockTime", fhcPara.openLockTime);
    query->bindValue(":OperationTime", fhcPara.OptBussinessTime);
    query->bindValue(":RemindTimeout", fhcPara.timeOutRemind);
    query->bindValue(":Interval", fhcPara.warnningDelayTime);
    query->bindValue(":NoManOpenTime", fhcPara.noManOpenLockTime);
    query->bindValue(":OpenFanTemp", fhcPara.fanRunTemperature);
    query->bindValue(":HumanDetectingInputType", fhcPara.signalModel);
    query->bindValue(":HumanDetectingType", fhcPara.peopleEquModel);
    query->bindValue(":LockType", fhcPara.lockModel);
    query->bindValue(":LockWhileIdle", fhcPara.kongCangLockorNot);
    query->bindValue(":FangQiewarning", fhcPara.fangQiewarnning);
    //更新照明参数
    query->bindValue(":Mode", zmPara.Model);
    query->bindValue(":startHour", zmPara.startHour);
    query->bindValue(":startMinute", zmPara.startMinute);
    query->bindValue(":stopHour", zmPara.endHour);
    query->bindValue(":stopMinute", zmPara.endMinute);
    query->bindValue(":manEnable", zmPara.manEnable);
    //更新LED节目
    query->bindValue(":lpmWelcome", temp->ledText[0]);
    query->bindValue(":lpmUsing", temp->ledText[1]);
    query->bindValue(":lpmMaintaining", temp->ledText[2]);
    //更新音乐参数
    query->bindValue(":volume", temp->soundVolume);
    query->bindValue(":welcome", temp->getSoundMd5(0));
    query->bindValue(":using", temp->getSoundMd5(1));
    query->bindValue(":maintaining", temp->getSoundMd5(2));
    query->bindValue(":reminding", temp->getSoundMd5(3));
    query->bindValue(":timeout",temp->getSoundMd5(4));
    query->bindValue(":goodbye",temp->getSoundMd5(5));
    query->bindValue(":doorOpen",temp->getSoundMd5(6));
    query->bindValue(":notLocked",temp->getSoundMd5(7));
    query->bindValue(":slideDoor",temp->getSoundMd5(8));
    //更新报警设置参数
    query->bindValue(":btnAlarmEnable", alarm.btnAlarmEnable);
    query->bindValue(":cutAlarmEnable", alarm.cutAlarmEnable);
    query->bindValue(":zhengDongAlarmEnable",alarm.zhengDongAlarmEnable);
    query->bindValue(":yanWuAlarmEnable",alarm.yanWuAlarmEnable);
    query->bindValue(":boLiAlarmEnable",alarm.boLiAlarmEnable);
    query->bindValue(":shuiQinAlarmEnable",alarm.shuiQinAlarmEnable);
    query->bindValue(":tempAlarmEnable",alarm.tempAlarmEnable);
    //加钞间参数
    query->bindValue(":userNum", fhcPara.userNum);
    query->bindValue(":systemAlarmStatus", fhcPara.systemAlarmStatus);
    query->bindValue(":setGuardDelayTime", fhcPara.setGuardDelayTime);
    query->bindValue(":isMonitorOrNot", fhcPara.isMonitorOrNot);
    query->bindValue(":inDoorModel", fhcPara.inDoorModel);
    query->bindValue(":outDoorModel", fhcPara.outDoorModel);

    query->bindValue(":doorCiAlarmEnable",alarm.doorCiAlarmEnable);
    query->bindValue(":existManAlarmEnable",alarm.existManAlarmEnable);

    query->bindValue(":mac", temp->net.macAddr);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
void frmMain::slotUpdateDatabase(Equipment *slave)
{
    QString mac = slave->equParaData.net.macAddr;
    if(checkExist("TParamater","Mac",mac))
    {
        Update_TParamater_Sql(slave);
    }
    else
    {
        Insert_TParamater_Sql(slave);
    }
}
void frmMain::btnWtiteNowTime()
{
    if(!checkPermission(guestPermission.permitAdjustTime))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许同步时间！");
        //添加日志记录
        InsertOneSystemLogAndShow("同步时间操作 操作员不允许同步时间！");
        return;
    }
    if(currentSlave == NULL)
        return;
    if(currentSlave->GetEquAll()->slaveStatus.connectStatus != 1)
    {
        return;
    }
    quint8 year = QDateTime::currentDateTime().date().year()-2000;
    quint8 month = QDateTime::currentDateTime().date().month();
    quint8 day = QDateTime::currentDateTime().date().day();
    quint8 hour = QDateTime::currentDateTime().time().hour();
    quint8 minute = QDateTime::currentDateTime().time().minute();
    quint8 second = QDateTime::currentDateTime().time().second();
    quint8 week = QDateTime::currentDateTime().date().dayOfWeek();
    emit sigWriteNowTime(year,month,day,hour,minute,second,week);
    currentSlave->slotReadSlaveTime();
    //添加日志记录
    InsertOneSystemLogAndShow("同步时间操作");
}
void frmMain::slotOpenDoor()
{
    if(!checkPermission(guestPermission.permitOpenDoor))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许远程开门！");
        return;
        //添加日志记录
        InsertOneSystemLogAndShow("开门操作 操作员不允许远程开门！");
    }

    if(currentSlave->GetEquAll()->slaveStatus.lockStatus)
    {
        myHelper::ShowMessageBoxError("智能门禁已锁闭，操作无法完成！");
        //添加日志记录
        InsertOneSystemLogAndShow("开门操作 智能门禁已锁闭，操作无法完成！");
    }
    else
    {
        emit sigOpenDoor(true);
        //添加日志记录
        InsertOneSystemLogAndShow("开门操作");
    }
}
void frmMain::slotShowOpenDoorMsg(quint8 val)
{
    if(val)
        myHelper::ShowMessageBoxError("远程开门失败！");
    else
        myHelper::ShowMessageBoxInfo("远程开门完成");
}
void frmMain::slotShowCloseLockMsg(quint8 val,quint8 result)
{
    if(val)
        myHelper::ShowMessageBoxError("操作失败！");
    else
    {
        if(currentSlave->GetEquAll()->slaveStatus.gateStatus)
        {
            if(currentSlave->GetEquAll()->slaveStatus.lockStatus)
            {
                myHelper::ShowMessageBoxInfo("远程解锁顺利完成。");
            }
            else
            {
                myHelper::ShowMessageBoxInfo("远程闭锁顺利完成。智能门禁会在锁条件（门关闭，锁舌到位）具备后锁闭舱体。");
            }
        }
        else
        {
            if(currentSlave->GetEquAll()->slaveStatus.lockStatus)
            {
                myHelper::ShowMessageBoxInfo("远程解锁顺利完成。");
            }
            else
            {
                myHelper::ShowMessageBoxInfo("远程闭锁顺利完成。");
            }
        }
    }

}
void frmMain::slotShowOpenLockMsg(quint8 val)
{
    if(val)
        myHelper::ShowMessageBoxError("远程开锁失败！");
    else
    {
        myHelper::ShowMessageBoxInfo("远程开锁顺利完成。");
    }

}
void frmMain::slotEmitSearchDone()
{
    udpClient->close();
    searchTimeOut->stop();
    emit sigAnalysisData();
    //emit SearchedEqument(&viewlist,&searchedAll);
}
void frmMain::slotShowReadMsgProcess()
{
    //frmReadSlaveMsg * readMsgProcess = new frmReadSlaveMsg();
    //readMsgProcess->exec();
}
void frmMain::slotInsertMsgToLog(QList<SlaveMsg *> list)
{
    QList<FangHuCangLogItem *> fanghucangitemlist;
    fanghucangitemlist.clear();
    QDateTime datetime;
    QString tempDateTime;
    QString name;
    QString item;
    datetime = QDateTime::currentDateTime();
    tempDateTime = datetime.toString("yyyy-MM-dd hh:mm:ss");
    for(int i=0;i<list.count();i++)
    {
        int rowNum = ui->tableWidgetSlaveLog->rowCount();
        ui->tableWidgetSlaveLog->insertRow(rowNum);
        ui->tableWidgetSlaveLog->setItem(rowNum,0,new QTableWidgetItem(tempDateTime));
        name = currentSlave->GetEquNameMac();
        ui->tableWidgetSlaveLog->setItem(rowNum,1,new QTableWidgetItem(name));
        item = list.at(i)->getLogItem();
        ui->tableWidgetSlaveLog->setItem(rowNum,2,new QTableWidgetItem(item));
        //插入到防护舱日志中
        FangHuCangLogItem *tempLog = new FangHuCangLogItem();
        tempLog->dateTime = datetime;
        tempLog->logItem = item;
        tempLog->name = name;

        fangHuCangLogList.append(tempLog);
        fanghucangitemlist.append(tempLog);
        qApp->processEvents();//防止循环次数太多界面卡死
    }
    //滚动条滑动到最后
    ui->tableWidgetSlaveLog->scrollToBottom();
    for(int i=0;i<fanghucangitemlist.count();i++)
    {
        InsetItemSlaveLogToDB(fanghucangitemlist.at(i));
        qApp->processEvents();//防止循环次数太多界面卡死
    }
    fanghucangitemlist.clear();
}
//全部显示系统日志
void frmMain::slotShowSystemLog(QList<SystemLogItem *> list)
{

    for(int i=0;i<list.count();i++)
    {
        int rowNum = ui->systemLog->rowCount();
        ui->systemLog->insertRow(rowNum);
        ui->systemLog->setItem(rowNum,0,new QTableWidgetItem(list.at(i)->dataTime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->systemLog->setItem(rowNum,1,new QTableWidgetItem(list.at(i)->LogItem));

    }
    ui->tabWidgetLog->setCurrentIndex(0);
    //滚动条滑动到最后
    ui->systemLog->scrollToBottom();
    for(int i=0;i<list.count();i++)
    {
        InsetItemSystemLogToDB(list.at(i));
    }
}
//插入一条系统日志
void frmMain::slotInsertItemSystemLog(SystemLogItem * item)
{
    systemLogList.append(item);
    int rowNum = ui->systemLog->rowCount();
    ui->systemLog->insertRow(rowNum);
    ui->systemLog->setItem(rowNum,0,new QTableWidgetItem(item->dataTime.toString("yyyy-MM-dd hh:mm:ss")));
    ui->systemLog->setItem(rowNum,1,new QTableWidgetItem(item->LogItem));
    //滚动条滑动到最后
    ui->tabWidgetLog->setCurrentIndex(1);
    ui->systemLog->scrollToBottom();
    InsetItemSystemLogToDB(item);
}
void frmMain::InsetItemSlaveLogToDB(FangHuCangLogItem * item)
{

    slaveLogquery->prepare("insert into TFangHuCangLog values (?,?,?)");
    //日期时间
    //QString str = fangHuCangLogList.at(i)->dateTime.toString("yyyy-MM-dd hh:mm:ss");
    slaveLogquery->addBindValue(item->dateTime);
    //防护舱名称
    slaveLogquery->addBindValue(item->name);
    //日志内容
    slaveLogquery->addBindValue(item->logItem);

    if(!slaveLogquery->exec())
    {
        QLOG_ERROR() <<slaveLogquery->lastError();
    }

}
void frmMain::InsetItemSystemLogToDB(SystemLogItem * item)
{

    systemLogquery->prepare("insert into TSystemLog values (?,?)");
    //日期时间
    systemLogquery->addBindValue(item->dataTime);
    //日志内容
    systemLogquery->addBindValue(item->LogItem);

    if(!systemLogquery->exec())
    {
        QLOG_ERROR() <<systemLogquery->lastError();
    }

}
//显示防护舱日志
void frmMain::slotShowFangHuCangLog(QList<FangHuCangLogItem *> list)
{

    for(int i=0;i<list.count();i++)
    {
        int rowNum = ui->tableWidgetSlaveLog->rowCount();
        fangHuCangLogList.append(list.at(i));
        ui->tableWidgetSlaveLog->insertRow(rowNum);
        ui->tableWidgetSlaveLog->setItem(rowNum,0,new QTableWidgetItem(list.at(i)->dateTime.toString("yyyy-MM-dd hh:mm:ss")));
        ui->tableWidgetSlaveLog->setItem(rowNum,1,new QTableWidgetItem(list.at(i)->name));
        ui->tableWidgetSlaveLog->setItem(rowNum,2,new QTableWidgetItem(list.at(i)->logItem));

    }
    ui->tabWidgetLog->setCurrentIndex(0);
    //滚动条滑动到最后
    ui->tableWidgetSlaveLog->scrollToBottom();

    for(int i=0;i<list.count();i++)
    {
        InsetItemSlaveLogToDB(list.at(i));
    }
}
//插入一条防护舱日志
void frmMain::slotInsertItemFangHuCangLog(FangHuCangLogItem * item)
{

}
//led欢迎内容修改
void frmMain::slotbtnLed0TextChange()
{
    if(ui->txtBrowse0->isReadOnly())
    {
        ui->txtBrowse0->setReadOnly(false);
        ui->btnChange0->setText("取消");
        ui->btnLed0Ok->setVisible(true);
    }
    else
    {
        ui->txtBrowse0->setText(currentSlave->GetEquAll()->ledText[0]);
        ui->btnChange0->setText("更换");
        ui->txtBrowse0->setReadOnly(true);
        ui->btnLed0Ok->setVisible(false);
    }
}
//确认led欢迎内容修改
void frmMain::slotbtnSetLed0TextOk()
{
    ui->btnChange0->setText("更换");
    ui->txtBrowse0->setReadOnly(true);
    ui->btnLed0Ok->setVisible(false);
    //qDebug()<<ui->txtBrowse0->toPlainText();
    emit sigSetLedText(0,ui->txtBrowse0->toPlainText());
    //添加日志记录
    InsertOneSystemLogAndShow("修改led欢迎内容");
}
//led使用中内容修改
void frmMain::slotbtnLed1TextChange()
{
    if(ui->txtBrowse1->isReadOnly())
    {
        ui->txtBrowse1->setReadOnly(false);
        ui->btnChange1->setText("取消");
        ui->btnLed1Ok->setVisible(true);
    }
    else
    {
        ui->txtBrowse1->setText(currentSlave->GetEquAll()->ledText[1]);
        ui->btnChange1->setText("更换");
        ui->txtBrowse1->setReadOnly(true);
        ui->btnLed1Ok->setVisible(false);
    }
}
//确认led使用中内容修改
void frmMain::slotbtnSetLed1TextOk()
{
    ui->btnChange1->setText("更换");
    ui->txtBrowse1->setReadOnly(true);
    ui->btnLed1Ok->setVisible(false);

    emit sigSetLedText(1,ui->txtBrowse1->toPlainText());
    //添加日志记录
    InsertOneSystemLogAndShow("修改led使用中内容");
}
//led暂停使用内容修改
void frmMain::slotbtnLed2TextChange()
{
    if(ui->txtBrowse2->isReadOnly())
    {
        ui->txtBrowse2->setReadOnly(false);
        ui->btnChange2->setText("取消");
        ui->btnLed2Ok->setVisible(true);
    }
    else
    {
        ui->txtBrowse2->setText(currentSlave->GetEquAll()->ledText[2]);
        ui->btnChange2->setText("更换");
        ui->txtBrowse2->setReadOnly(true);
        ui->btnLed2Ok->setVisible(false);
    }
}
//确认led暂停使用内容修改
void frmMain::slotbtnSetLed2TextOk()
{
    ui->btnChange2->setText("更换");
    ui->txtBrowse2->setReadOnly(true);
    ui->btnLed2Ok->setVisible(false);

    emit sigSetLedText(2,ui->txtBrowse2->toPlainText());
    //添加日志记录
    InsertOneSystemLogAndShow("修改led暂停使用内容");
}
void frmMain::slotBtnSetSoundVolume()
{
    emit sigSetSlaveVolume(ui->hsVoice->value());
    //添加日志记录
    InsertOneSystemLogAndShow("修改语音音量");
}

void frmMain::on_hsVoice_valueChanged(int value)
{
    QString strValue = QString::number(value);
    ui->lbVoice->setText(strValue);
}

void frmMain::readModuleData()
{
    //读取版本号和名称
    versionList.clear();
    if(!query->exec("select tmodule.[Name],TMODULE.[level],TVERSIONMANAGER.[Name] as version from TMODULE join TVERSIONMANAGER ON TMODULE.[level] = TVERSIONMANAGER.[Level]"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            SlaveVersion *tempVersion = new SlaveVersion();

            tempVersion->name     = query->value(0).toString();
            tempVersion->level    = query->value(1).toInt();
            tempVersion->version = query->value(2).toString();

            versionList.append(tempVersion);
        }
    }
    //moduleModel->setTable("TModule");

    //moduleModel->setQuery(QSqlQuery("select tmodule.[Name],TMODULE.[level],TVERSIONMANAGER.[Name] as version from TMODULE join TVERSIONMANAGER ON TMODULE.[level] = TVERSIONMANAGER.[Level]",database));
    //moduleModel->select();
    //moduleModel->setHeaderData(0,Qt::Horizontal,tr("名称"));
    //moduleModel->setHeaderData(1,Qt::Horizontal,tr("适用版本"));
    emit sigSendModuleTbModel(versionList,moduleList);
    frmManagerModel->slotSetVersionInfoList(versioninfoList);
    emit sigSendModuleTbModel(moduleModel);
}
void frmMain::readVoiceData()
{
    voiceLibrary.clear();
    voiceModel->setTable("TVoice");
    voiceModel->select();
    voiceModel->setHeaderData(0,Qt::Horizontal,tr("音乐名称"));
    if(!query->exec("select * from TVoice"))
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            VoiceInfoBank temp;
            temp.name = query->value(0).toString();
            temp.md5 = query->value(1).toString();
            temp.fileName = query->value(2).toString();
            temp.generation = query->value(3).toInt();
            voiceLibrary.append(temp);
        }
    }
    emit sigSendVoiceTbModel(voiceModel);
}
void frmMain::slotBtnVoiceManager()
{
    emit sigSendVoiceTbModel(voiceModel);

    frmVoiceManager->DisableorEnableEdit(checkPermission(guestPermission.permitEditSoundLiabrary));
    frmVoiceManager->setModal(true);
    frmVoiceManager->show();
}
void frmMain::slotShowSoundSaveWindow(paraData *temp)
{
    editedName="";
    frmReadSoundProcess->close();
    frmSaveSound->ClearText();
    frmSaveSound->exec();
    if(editedName!="")
    {
        QString fileName=QDateTime::currentDateTime().toString("yyyyMMddhhmmss")+".mp3";
        QFile file(sFilePath+fileName);
        //方式：Append为追加，WriteOnly，ReadOnly
        if (!file.open(QIODevice::WriteOnly))
        {
            myHelper::ShowMessageBoxError("无法创建文件");
            return;
        }
        QLOG_INFO()<<"文件大小："<<temp->mp3File.length();
        file.write(temp->mp3File.data(),temp->mp3File.length());
        file.flush();
        file.close();
        myHelper::ShowMessageBoxInfo("保存完成。");
        temp->mp3File.clear();
        QByteArray md5 = myHelper::GetFileMd5(sFilePath+fileName);
        //保存完文件后写入数据库
        UpdateVoice(editedName,md5.toHex(),fileName);
        readVoiceData();//重新读取音乐库
        slotupdateSlaveVoiceParaDisplay(currentSlave->GetEquAll());//更新显示
    }
}
void frmMain::UpdateVoice(QString name,QString md5,QString fileName)
{
    if(checkExist("TVoice","Name",name))
    {
        //Update_TVoice_Sql(editedName,md5,fileName);
        myHelper::ShowMessageBoxError("文件已存在！");
        return;
    }
    else
    {
        Insert_TVoice_Sql(name,fileName,md5);
    }
}
//通过名称查找对应的文件名和md5
void frmMain::Select_TVoice_FileName_Md5(QString name,QString &fileName,QString &md5)
{
    if(!checkExist("TVoice","Name",name))
    {
        return;
    }
    //更新项目
    query->prepare("SELECT FileName,Md5 FROM TVoice WHERE Name=:name");
    query->bindValue(":name", name);
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            fileName = query->value(0).toString();
            md5 = query->value(1).toString();
        }
    }

}
//通过Md5查找对应的文件名和名字
void frmMain::Select_TVoice_FileName_Name(QString &name,QString &fileName,QString md5)
{
    //更新项目
    query->prepare("SELECT FileName,Name FROM TVoice WHERE Md5=:md5");
    query->bindValue(":md5", md5);
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            fileName = query->value(0).toString();
            name = query->value(1).toString();
        }
    }

}
//通过名称查找对应的文件名和md5
void frmMain::Select_TVoice_FileName(QString &fileName,QString md5)
{
    if(!checkExist("TVoice","Md5",md5))
    {
        return;
    }
    //更新项目
    query->prepare("SELECT FileName FROM TVoice WHERE Md5=:md5");
    query->bindValue(":md5", md5);
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
    else
    {
        while(query->next())
        {
            fileName = query->value(0).toString();
        }
    }

}
void frmMain::changeVoice(quint8 id)
{
    if(!checkPermission(guestPermission.permitEditVoicePar))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许更改声音！");
        //添加日志记录
        InsertOneSystemLogAndShow("更改声音 操作员不允许更改声音！");
        return;
    }
    editedName="";
    FrmSelectSound *frmSelectSound = new FrmSelectSound();
    connect(frmSelectSound,SIGNAL(sigoperate(QString)),this,SLOT(sloteditedName(QString)));
    frmSelectSound->setTreeViewModel(voiceModel);
    frmSelectSound->exec();
    if(editedName!="")
    {
        QString fileName="";
        QString md5="";
        //读取文件的名称和md5
        Select_TVoice_FileName_Md5(editedName,fileName,md5);
        emit sigChangeSound(id,fileName,md5);
        //添加日志记录
        InsertOneSystemLogAndShow("更改声音操作");
    }
}
void frmMain::slotBtnChangeHySound()
{
    changeVoice(0);
    //添加日志记录
    InsertOneSystemLogAndShow("更改欢迎声音操作");
}
void frmMain::slotBtnChangesyzSound()
{
    changeVoice(1);
    //添加日志记录
    InsertOneSystemLogAndShow("更改使用中声音操作");
}
void frmMain::slotBtnChangeWhzSound()
{
    changeVoice(2);
    //添加日志记录
    InsertOneSystemLogAndShow("更改维护中声音操作");
}
void frmMain::slotBtnChangeTimeoutWarnSound()
{
    changeVoice(3);
    //添加日志记录
    InsertOneSystemLogAndShow("更改超时提醒声音操作");
}
void frmMain::slotBtnChangeTimeoutTiShiSound()
{
    changeVoice(4);
    //添加日志记录
    InsertOneSystemLogAndShow("更改超时提示声音操作");
}
void frmMain::slotBtnChangeOutDoorSound()
{
    changeVoice(5);
    //添加日志记录
    InsertOneSystemLogAndShow("更改再见声音操作");
}
void frmMain::slotBtnChangeOpenDoorSound()
{
    changeVoice(6);
    //添加日志记录
    InsertOneSystemLogAndShow("更改开门声音操作");
}
void frmMain::slotBtnChangeFailLockSound()
{
    changeVoice(7);
    //添加日志记录
    InsertOneSystemLogAndShow("更改未锁好声音操作");
}
void frmMain::slotBtnChangeLaDoorSound()
{
    changeVoice(8);
    //添加日志记录
    InsertOneSystemLogAndShow("更改拉门声音操作");
}
void frmMain::slotShowReadSoundProcess(quint64 size)
{
    frmReadSoundProcess->setWindowFlags(frmReadSoundProcess->windowFlags() | Qt::WindowStaysOnTopHint);
    frmReadSoundProcess->setProcessMaxValue(size);
    frmReadSoundProcess->show();
}
void frmMain::slotShowUseModuleProcess(quint64 size)
{
    frmReadSoundProcess->setWindowFlags(frmReadSoundProcess->windowFlags() | Qt::WindowStaysOnTopHint);
    frmReadSoundProcess->setPBarMainMaxValue(size);
    frmReadSoundProcess->show();
}
void frmMain::slotDeleteVoice(QString name)
{
    delete_TVoice_Sql(name);
    readVoiceData();
    if(currentSlave!=NULL)
    {
        currentSlave->slotReadAllSoundMd5();
    }
}
void frmMain::slotAddVoice(QString filePath,QString name)
{
    if(name=="")
    {
        return;
    }
    //读取文件
    QByteArray fileData=NULL;
    QString fileName;
    QByteArray filemd5 = myHelper::GetFileMd5(filePath);
    //判断音乐库中是否存在md5相同的
    query->prepare("SELECT Md5 From TVoice");
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
        emit sigAddVoiceEnd(0);
        //myHelper::ShowMessageBoxError("读取数据库错误！");
        return;
    }
    else
    {
        while(query->next())
        {
            QString md5 = query->value(0).toString();
            if(md5 == filemd5.toHex())
            {
                emit sigAddVoiceEnd(1);
                //myHelper::ShowMessageBoxInfo("音乐文件已经存在！");
                return;
            }
        }
    }

    QFile *localFile = new QFile(filePath);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    fileData = localFile->readAll();
    //写入文件
    fileName=QDateTime::currentDateTime().toString("yyyyMMddhhmmss")+".mp3";
    QFile file(sFilePath+fileName);
    //方式：Append为追加，WriteOnly，ReadOnly
    if (!file.open(QIODevice::WriteOnly))
    {
        emit sigAddVoiceEnd(2);
        //myHelper::ShowMessageBoxError("无法创建文件");
        return;
    }

    file.write(fileData.data(),fileData.length());
    file.flush();
    file.close();


    //保存完文件后写入数据库
    UpdateVoice(name,filemd5.toHex(),fileName);
    readVoiceData();//重新读取音乐库
}
void frmMain::slotShowAddVoiceWindow()
{
    //frmAddSound->show();
}
void frmMain::slotBtnReadHySound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[0].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadHySound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取欢迎语音");
    }
}
void frmMain::slotBtnReadSyzSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[1].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadSyzSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取使用中语音");
    }
}
void frmMain::slotBtnReadWhzSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[2].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadWhzSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取维护中语音");
    }
}
void frmMain::slotBtnReadTimeoutWarnSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[3].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadTimeoutWarnSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取超时提醒语音");
    }
}
void frmMain::slotBtnReadTimeoutTiShiSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[4].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadTimeoutTiShiSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取超时提示语音");
    }
}
void frmMain::slotBtnReadFailLockSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[7].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadFailLockSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取未锁好语音");
    }
}
void frmMain::slotBtnReadOpenDoorSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[6].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadOpenDoorSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取开门语音");
    }
}
void frmMain::slotBtnReadOutDoorSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[5].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadOutDoorSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取再见语音");
    }
}
void frmMain::slotBtnReadLaDoorSound()
{
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[8].toHex();
    if(checkExist("TVoice","Md5",md5))
    {
        myHelper::ShowMessageBoxInfo("声音文件已经存在！不允许重复读取！");
    }
    else
    {
        emit sigReadLaDoorSound();
        //添加日志记录
        InsertOneSystemLogAndShow("读取拉门语音");
    }
}
void frmMain::slotBtnPlayHySound()
{
    //emit sigPlaySlaveSound(1);
    //添加日志记录
    InsertOneSystemLogAndShow("试听欢迎语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[0].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(1);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlaySyzSound()
{
    //emit sigPlaySlaveSound(2);
    //添加日志记录
    InsertOneSystemLogAndShow("试听使用中语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[1].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(2);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlayWhzSound()
{
    //emit sigPlaySlaveSound(3);
    //添加日志记录
    InsertOneSystemLogAndShow("试听维护中语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[2].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(3);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlayTimeoutWarnSound()
{
    //emit sigPlaySlaveSound(4);
    //添加日志记录
    InsertOneSystemLogAndShow("试听超时提醒语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[3].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(4);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlayTimeoutTiShiSound()
{
    //emit sigPlaySlaveSound(5);
    //添加日志记录
    InsertOneSystemLogAndShow("试听超时提示语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[4].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(5);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlayOutDoorSound()
{
    //emit sigPlaySlaveSound(6);
    //添加日志记录
    InsertOneSystemLogAndShow("试听再见语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[5].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(6);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlayOpenDoorSound()
{
    //emit sigPlaySlaveSound(7);
    //添加日志记录
    InsertOneSystemLogAndShow("试听开门语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[6].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(7);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlayFailLockSound()
{
    //emit sigPlaySlaveSound(8);
    //添加日志记录
    InsertOneSystemLogAndShow("试听未锁好语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[7].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(8);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlayLaDoorSound()
{
    //emit sigPlaySlaveSound(9);
    //添加日志记录
    InsertOneSystemLogAndShow("试听拉门语音");
    //return;
    //phonon play
    paraData* pdat = currentSlave->GetEquAll();
    QString md5= pdat->soundMd5[8].toHex();
    QString fileName="";
    emit sigPlaySlaveSound(9);
    Select_TVoice_FileName(fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
}
void frmMain::slotBtnPlaySound(QString name)
{
    //phonon play
    QString md5="";
    QString fileName="";
    Select_TVoice_FileName_Md5(name,fileName,md5);
    //mMediaObj->clear();
    //mMediaObj->stop();
    mMediaObj->setCurrentSource(sFilePath+fileName);
    mMediaObj->play();
    //添加日志记录
    InsertOneSystemLogAndShow("试听语音"+name);
}
void frmMain::slotShowManagerModuleWindows()
{
    if(!checkPermission(guestPermission.permitEditModule))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许管理模板！");
        return;
    }
    frmManagerModel->setVoiceModel(voiceModel);
    frmManagerModel->setModal(true);
    frmManagerModel->show();
}
void frmMain::slotShowUseModuleWindows()
{
    frmUseModule->setModal(true);
    frmUseModule->show();
    //frmUseModule->exec();
}
void frmMain::slotShowAddModuleWindows()
{/*
    paraData *para = new paraData();
    frmModulePara->SetPara("新建模板",para,0);
    frmModulePara->SetTitleText("新建模板参数");
    //frmModulePara->show();
    frmModulePara->exec();*/
}

void frmMain::slotShowEditModuleWindows(QModelIndex index)
{/*
    QString name = moduleModel->data(index).toString();
    paraData * tempPara=NULL;
    for(int i=0;i<moduleList.count();i++)
    {
        if(moduleList.at(i)->name == name)
        {
            tempPara = &moduleList.at(i)->pdat;
            break;
        }
    }
    if(tempPara==NULL)
        return;
    frmModulePara->SetPara(name,tempPara,1);
    frmModulePara->SetTitleText("修改模板参数");
    frmModulePara->show();
    */
}
void frmMain::slotChangeModuleSound(quint8 id)
{/*
    editedName="";
    frmSelectSound->exec();
    if(editedName!="")
    {
        QString fileName="";
        QString md5="";
        //读取文件的名称和md5
        Select_TVoice_FileName_Md5(editedName,fileName,md5);
        emit sigChangeModuleSound(id,md5);
    }*/
}
void frmMain::slotUpdateModuleSoundName(paraData* pdata)
{
    getSlaveSoundName(pdata);
}
void frmMain::slotSaveParaModule(paraModule * pModule)
{
    if(checkExist("TMparamater","Name",pModule->name))
    {
        Update_TMparamater_Sql(pModule->name,&pModule->pdat);
        //Update_TModule_sql(name);
    }
    else
    {
        moduleList.append(pModule);
        Insert_TMparamater_Sql(pModule->name,&pModule->pdat);
        Insert_TModule_Sql(pModule);
    }
    for(int i=0;i<9;i++)
    {
        QString fileName="";
        QString md5 = pModule->pdat.soundMd5[i].toHex();
        Select_TVoice_FileName(fileName,md5);
        pModule->pdat.voiceFileName[i]=fileName;

    }
    readModuleData();
}
void frmMain::slotDeleteModule(QModelIndex index)
{
    QString name = versionList.at(index.row())->name;
    delete_TModule_Sql(name);
    delete_TMparamater_Sql(name);
    for(int i=0;i<moduleList.count();i++)
    {
        if(moduleList.at(i)->name == name)
        {
            moduleList.removeAt(i);
        }
    }
    readModuleData();
}
void frmMain::slotUseModule(QModelIndex index)
{
    if(currentSlave == NULL)
    {
        myHelper::ShowMessageBoxInfo("请先选择设备！");
        return;
    }
    if(currentSlave->GetEquAll()->slaveStatus.connectStatus != 1)
    {
        myHelper::ShowMessageBoxInfo("设备未连接！");
        return;
    }
    else
    {
        QString name = moduleModel->data(index).toString();
        paraData * tempPara=NULL;
        for(int i=0;i<moduleList.count();i++)
        {
            if(moduleList.at(i)->name == name)
            {
                tempPara = &moduleList.at(i)->pdat;
                break;
            }
        }
        if(tempPara==NULL)
            return;

        emit sigUseModule(tempPara);
    }
}
void frmMain::slotUseModuleSound(paraData *temp, quint8 id)
{
    QString fileName="";
    QString md5 = temp->soundMd5[id].toHex();
    Select_TVoice_FileName(fileName,md5);
    if(fileName  == "")
    {
        return;
    }
    emit sigChangeSound(id,fileName,md5);
    QLOG_INFO()<<"set sound fileName = "<<fileName<<"id = "<<id;

}

void frmMain::addModuleVoice(QString filePath,QString name)
{
    //读取文件
    QByteArray fileData=NULL;
    QString fileName;
    QByteArray filemd5 = myHelper::GetFileMd5(filePath);
    //判断音乐库中是否存在md5相同的
    query->prepare("SELECT Md5 From TVoice");
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
        return;
    }
    else
    {
        while(query->next())
        {
            QString md5 = query->value(0).toString();
            if(md5 == filemd5.toHex())
            {
                QLOG_INFO()<<"MD5:"<<md5;
                QLOG_INFO()<<"fileMd5:"<<filemd5.toHex();
                return;
            }
        }
    }

    QFile *localFile = new QFile(filePath);
    if(!localFile->open(QFile::ReadOnly))
    {
        return;
    }
    fileData = localFile->readAll();
    QLOG_INFO()<<"读取模板的音乐保存。";
    QLOG_INFO()<<"写入音乐文件的大小："<<fileData.size();
    //QLOG_INFO()<<"写入音乐文件内容:"<<fileData.toHex();
    //写入文件
    fileName=QDateTime::currentDateTime().toString("yyyyMMddhhmmss")+".mp3";
    QFile file(sFilePath+fileName);
    //方式：Append为追加，WriteOnly，ReadOnly
    if (!file.open(QIODevice::WriteOnly))
    {
        //myHelper::ShowMessageBoxError("无法创建文件");
        return;
    }
    //QDataStream out(&file);
    //out<<fileData;
    file.write(fileData.data(),fileData.length());
    file.flush();
    file.close();


    //保存完文件后写入数据库
    //UpdateVoice(name,filemd5.toHex(),fileName);
    if(checkExist("TVoice","Name",name))
    {
        return;
    }
    else
    {
        Insert_TVoice_Sql(name,fileName,filemd5.toHex());
    }

    readVoiceData();//重新读取音乐库
}
//导入的参数写入数据库
void frmMain::slotSaveModulePara(paraModule * tempModule)
{
    if(checkExist("TMparamater","Name",tempModule->name))
    {
        myHelper::ShowMessageBoxInfo("模板名称相同不能导入！");
        return;
    }
    else
    {
        moduleList.append(tempModule);
        Insert_TMparamater_Sql(tempModule->name,&tempModule->pdat);
        Insert_TModule_Sql(tempModule);
    }
    //将导入的音乐写入音乐库，如果存在则不导入 最后全部删除
    for(int i=0;i<9;i++)
    {
        QString filepath="./temp/"+QString::number(i)+".mp3";
        addModuleVoice(filepath,tempModule->pdat.soundName[i]);
    }
    QLOG_INFO()<<"语音内容保存完成，重新读取模板库";
    readModuleData();
}

//报警后弹出页面
void frmMain::slotShowWarningWindowAndPlaySound(Equipment * slave)
{
    frmAlarm->setSlave(currentSlave);
    //frmAlarm->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    //frmAlarm->exec();
    frmAlarm->setModal(true);
    frmAlarm->show();
    QLOG_INFO()<<"alarm!";
}


void frmMain::on_btnSetAlarmPara_clicked()
{
    frmEditAlarmPara * frmEditAlarm = new frmEditAlarmPara();
    connect(frmEditAlarm,SIGNAL(sigWriteAlarmPara(paraAlarm*)),currentSlave,SLOT(slotSetAlarmPara(paraAlarm*)));
    frmEditAlarm->initPara(currentSlave->GetEquAll()->alarmPara,0);
    frmEditAlarm->setModal(true);
    frmEditAlarm->show();
}
//设置报警参数
void frmMain::slotSetAlarmPara(paraAlarm* alarmPara)
{

}
void frmMain::keyPressEvent(QKeyEvent *e)
{
}

void frmMain::slotSelectionChanged(QItemSelection x,QItemSelection y)
{
    QModelIndex index = ui->treeView->currentIndex();
    slotchangeSlaveDisplay(index);
}
void frmMain::connectIconChanged()
{
    currFrame = connectingGif->currentPixmap();
    ui->tbStatus->setIcon(currFrame);
}
//批量修改区域内的照明参数
void frmMain::slotEditAreaSlaveZhaoMingPara()
{
    paraZhaoMing zmPara;
    frmEditZhaoMingPara *frmEditZhaoMing = new frmEditZhaoMingPara();
    connect(frmEditZhaoMing,SIGNAL(sigSaveAreaZhaoMingPara(paraZhaoMing*)),this,SLOT(slotSaveAreaSlaveZhaoMingPara(paraZhaoMing*)));
    frmEditZhaoMing->initPara(zmPara,2);
    //emit sigZhaoMingPara(zmPara,2);
    frmEditZhaoMing->setModal(true);
    frmEditZhaoMing->show();
    //添加日志记录
    InsertOneSystemLogAndShow("批量修改区域内的照明参数");
}
void frmMain::slotSaveAreaSlaveZhaoMingPara(paraZhaoMing * zmpara)
{
    QList<TreeViewItem*> slaveList = model->GetChildsList(ui->treeView->currentIndex());
    for(int i=0;i<slaveList.count();i++)
    {
        Equipment *slave = slaveList.at(i)->GetSlave();
        //只修改门禁的照明参数
        if(slave->equParaData.slaveType == 0)
        {
            slave->setZhaoMingPara(zmpara);
        }
    }
}
//批量修改区域内的工作参数
void frmMain::slotEditAreaSlaveWorkPara()
{
    paraFangHuCang fhcpara;
    frmEditParamater * frmEditSlavePara = new frmEditParamater();
    connect(frmEditSlavePara,SIGNAL(sigSaveAreaFangHuCangPara(paraFangHuCang*)),this,SLOT(slotSaveAreaSlaveWorkPara(paraFangHuCang*)));
    frmEditSlavePara->initPara(fhcpara,2);
    //emit sigEditFangHuCangPara(fhcpara,2);
    frmEditSlavePara->setModal(true);
    frmEditSlavePara->show();
    //添加日志记录
    InsertOneSystemLogAndShow("批量修改区域内的工作参数");

}
//显示联系方式窗口
void frmMain::slotShowContactWindow()
{

    //frmInfo->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    frmInfo->setModal(true);
    frmInfo->show();
}
void frmMain::slotSaveAreaSlaveWorkPara(paraFangHuCang * fhcpara)
{
    QList<TreeViewItem*> slaveList = model->GetChildsList(ui->treeView->currentIndex());
    for(int i=0;i<slaveList.count();i++)
    {
        Equipment *slave = slaveList.at(i)->GetSlave();
        //只修改门禁的工作参数
        if(slave->equParaData.slaveType == 0)
        {
            slave->setWorkPara(fhcpara);
        }
    }
}
void frmMain::writeSettings()
{
    //读取windows保存的状态
    QString fileName = myApp::AppPath+"AppConfig.ini";
    QSettings *settings = new QSettings(fileName,QSettings::IniFormat);
    settings->beginGroup("MainWindow");
    //settings->setValue("size", size());
    settings->setValue("splitter", ui->splitter->saveState());
    settings->setValue("splitter_2", ui->splitter_2->saveState());
    settings->setValue("splitter_3",ui->splitter_3->saveState());
    //settings->setValue("max",max);
    settings->endGroup();
}
void frmMain::readSettings()
{
    //还原上次保存的windows状态
    QString fileName = myApp::AppPath+"AppConfig.ini";
    QSettings *settings = new QSettings(fileName,QSettings::IniFormat);
    settings->beginGroup("MainWindow");
    //resize(settings->value("size", QSize(this->window()->width(), this->window()->height())).toSize());
    //max = settings->value("max").toBool();
    ui->splitter->restoreState(settings->value("splitter").toByteArray());
    ui->splitter_2->restoreState(settings->value("splitter_2").toByteArray());
    ui->splitter_3->restoreState(settings->value("splitter_3").toByteArray());
    settings->endGroup();
    //添加日志记录
    InsertOneSystemLogAndShow("读取配置信息");
}
void frmMain::slotShowEditUserPwdWindow()
{
    frmEditUserPwd->setCurrentUser(&currentUser);
    frmEditUserPwd->InitDisplay();
    frmEditUserPwd->setModal(true);
    frmEditUserPwd->show();
}
void frmMain::slotUpdateUserPwd()
{
    QString updateUserpwdSql = "update UserInfo set UserPwd=:userPwd where UserName = :userName";
    query->prepare(updateUserpwdSql);
    query->bindValue(":userPwd", currentUser.userPwd);
    query->bindValue(":userName", currentUser.userName);
    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
        return;
    }
    //添加日志记录
    InsertOneSystemLogAndShow("修改"+currentUser.userName+"密码");
}
void frmMain::slotShowEditUserPermissionWindow()
{
    frmEditUserPermission->initPara(guestPermission);
    frmEditUserPermission->setModal(true);
    frmEditUserPermission->show();
}
void frmMain::slotEditUserPermission(UserPermission *usr)
{
    guestPermission = *usr;

    //更新项目
    query->prepare(update_TPuser_sql);
    //更新操作员权限
    query->bindValue(":permitEditSystemPar", guestPermission.permitEditSystemPar);
    query->bindValue(":permitEditAreaInfo", guestPermission.permitEditAreaInfo);
    query->bindValue(":permitRegFangHuCang", guestPermission.permitRegFangHuCang);
    query->bindValue(":permitLogoutFangHuCang", guestPermission.permitLogoutFangHuCang);
    query->bindValue(":permitEditSoundLiabrary", guestPermission.permitEditSoundLiabrary);
    query->bindValue(":permitEditModule", guestPermission.permitEditModule);
    query->bindValue(":permitEditFangHuCangPar", guestPermission.permitEditFangHuCangPar);
    query->bindValue(":permitEditZhaoMingPar", guestPermission.permitEditZhaoMingPar);
    query->bindValue(":permitEditVoicePar", guestPermission.permitEditVoicePar);
    query->bindValue(":permitAdjustTime", guestPermission.permitAdjustTime);
    query->bindValue(":permitOpenDoor", guestPermission.permitOpenDoor);
    query->bindValue(":permitUseorNotusedFangHuCang", guestPermission.permitUseorNotusedFangHuCang);
    query->bindValue(":permitLockorOpenFangHuCang", guestPermission.permitLockorOpenFangHuCang);
    query->bindValue(":permitEditLedText", guestPermission.permitEditLedText);
    query->bindValue(":permitChangeAdmin", guestPermission.permitChangeAdmin);

    query->bindValue(":Level",guestPermission.level);

    if(!query->exec())
    {
        QLOG_ERROR() <<query->lastError();
    }
}
void frmMain::slotOpenHelpFile()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(myApp::AppPath+"Helper.doc").absoluteFilePath()));
}
void frmMain::slotEnableOrDisableSlave()
{
    if(!checkPermission(guestPermission.permitUseorNotusedFangHuCang))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许启用/停用防护舱！");
        InsertOneSystemLogAndShow("操作员不允许启用/停用防护舱！");
        return;
    }
    switch(currentSlave->equParaData.slaveType)
    {
    case 0://防护舱
    {
        InsertOneSystemLogAndShow("启用/停用防护舱！");
        emit sigEanbleOrDisableSlave();
    }
        break;
    case 1://加钞间
    {
        if(ui->btnEnOrDisAbleFangHuCang->text()=="设防")
        {
            currentSlave->slotSetSlaveAlarmStatus(1);
        }
        else if(ui->btnEnOrDisAbleFangHuCang->text()=="撤防")
        {
            currentSlave->slotSetSlaveAlarmStatus(0);
        }
    }
        break;
    case 0xff:\
        break;
    default:
        break;
    }

}
void frmMain::slotRemoteCloseLock()
{
    if(!checkPermission(guestPermission.permitLockorOpenFangHuCang))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许闭锁/解锁防护舱！");
        InsertOneSystemLogAndShow("操作员不允许闭锁/解锁防护舱！");
        return;
    }
    //添加日志记录
    InsertOneSystemLogAndShow("闭锁/解锁防护舱！");
    sigRemoteCloseLock();
}
void frmMain::slotReadSlaveMsg()
{
    sigReadSlaveMsg();
}
void frmMain::slotShowSearchSlaveLogWindow()
{
    frmSearchSlaveLog * frmsearchSlaveLog = new frmSearchSlaveLog();
    frmsearchSlaveLog->setTitle("防护舱记录查询");
    frmsearchSlaveLog->setDateTime(QDateTime::currentDateTime().addMonths(-1));
    frmsearchSlaveLog->setSlaveList(viewlist);
    frmsearchSlaveLog->setDatebaseQuery(query);
    frmsearchSlaveLog->setModal(true);
    frmsearchSlaveLog->show();
    //添加日志记录
    InsertOneSystemLogAndShow("防护舱记录查询");
}
void frmMain::slotShowSearchSystemLogWindow()
{
    frmSearchSystemLog * frmsearchSystemLog = new frmSearchSystemLog();
    frmsearchSystemLog->setDateTime(QDateTime::currentDateTime().addMonths(-1));
    frmsearchSystemLog->setDatebaseQuery(query);
    frmsearchSystemLog->setModal(true);
    frmsearchSystemLog->show();
    //添加日志记录
    InsertOneSystemLogAndShow("软件操作记录查询");
}
void frmMain::InsertOneSystemLogAndShow(QString item)
{
    //加入日志
    SystemLogItem *tempLog = new SystemLogItem();
    QString fanghucang="";
    QString slaveType="";

    if(currentSlave != NULL)
    {
        fanghucang = currentSlave->GetEquNameMac();
        switch (currentSlave->equParaData.slaveType) {
        case 0:
            slaveType="防护舱:";
            break;
        case 1:
            slaveType="加钞间:";
            break;
        default:
            break;
        }
    }
    else
    {
        fanghucang = "";
    }
    tempLog->LogItem = currentUser.userName+" "+item+" "+slaveType+fanghucang;
    //systemLogList.append(tempLog);
    slotInsertItemSystemLog(tempLog);
}
void frmMain::ClientConnect(int handle,QTcpSocket *client)
{
    if(tcpClient==NULL)
    {
        tcpClient = client;
        //tcpClientList.append(client);
        connect(client,SIGNAL(readyRead()),this,SLOT(receiveData()));
        sendTcpPacketage(1);
    }

}
void frmMain::sendTcpPacketage(quint8 func)
{
    QByteArray datagram;
    QDataStream out(&datagram,QIODevice::WriteOnly);
    uchar header=func;
    quint8 dataCount=0;
    uchar D[100]={0};
    uchar crc=0;
    switch(func)
    {
    case 1:
        dataCount=2;
        D[0]=0;
        D[1]=0;
    break;
    case 3:
        dataCount=2;
        D[0]=0;
        D[1]=0;
    break;
    default:
        return;
    break;
    }
    //写入包头
    out<<header;
    //写入数据
    for(int i=0;i<dataCount;i++)
    {
        out<<D[i];
    }
    crc = getCRC((uchar *)datagram.data(),dataCount+1);

    out<<crc;

    tcpClient->write(datagram);
    tcpClient->flush();//立即发送
}
//接收数据并写入缓冲区
void frmMain::receiveData()
{
    static QByteArray data=NULL;
    static quint64 dataLength=0;

    while(tcpClient->bytesAvailable()>0||(!data.isEmpty()))
    {
        //写入缓冲区
        if (!comBuffer->isFull())
        {
            if(data.isEmpty())
            {
                data = tcpClient->readAll();
            }
            dataLength = comBuffer->write(data.data(), data.length());
            QLOG_INFO()<<"receive data:"<<data.toHex();
            if(dataLength!=data.length())
            {

                data.remove(0,dataLength);
                slotAnalysisTcpData();
                QLOG_WARN()<<"buffer is full，emit signal to read buf.";
                break;
            }
            else
            {
                data.clear();
                QLOG_INFO()<<"Data written to complete. ";
            }
        }
        else//如果没有这个等待，会有很多空循环，CPU会很高
        {
            comBuffer->waitNotFull();
            emit sigAnalysisData();
            QLOG_WARN()<<"buffer is full，emit signal to read buf.";
        }
    }
    slotAnalysisTcpData();
}
//分析接收的数据
void frmMain::slotAnalysisTcpData()
{
    searchedAll.clear();
    static quint8 startBuf=0;
    static quint8 func=0;
    static char dbuf[500];//读取控制器记录最多230个字节
    static quint8 ret=1;
    static quint16 blockSize=0;

    while(!comBuffer->isEmpty())
    {
        if (!comBuffer->isEmpty())
        {
            //处理完一帧数据后func置0 重新读取功能码
            if(func == 0)
            {
                comBuffer->read((char*)&dbuf[0], 1);
                QLOG_INFO()<<"dbuf[0]:"<<(quint8)dbuf[0];
                if((quint8)dbuf[0] < KEY)
                {
                    comBuffer->read((char*)&dbuf[0], 3);
                    continue;
                }
                func=((quint8)dbuf[0]-KEY);

                QLOG_INFO()<<"func:"<<func;

                //计算需要读取的数据的个数
                if(func == 1)
                {
                    blockSize = 71;
                    startBuf = 1;
                }
                else if(func ==3)
                {
                    blockSize = 71;
                    startBuf = 1;
                }
                else
                {
                    comBuffer->setEmpty();
                    break;
                }
            }
            //QLOG_INFO()<<"blockSize:"<<blockSize;
            //QLOG_INFO()<<"startBuf:"<<startBuf;
            if(comBuffer->getFreeSize()>=blockSize)
            {

                //读取剩余数据并处理
                comBuffer->read((char*)&dbuf[startBuf],blockSize);

                paraData *pdata=new paraData();
                if(!FunctTcp1((char*)&dbuf,pdata))
                {
                    if(func == 1)
                    {
                        searchedAll.append(pdata);
                        //判断是否已经存在列表中
                        disconnect(tcpClient,SIGNAL(readyRead()),this,SLOT(receiveData()));
                        if(checkExist("TSlave","Mac",pdata->net.macAddr))
                        {
                            //如果存在直接连接
                            for(int i=0;i<viewlist.count();i++)
                            {
                                if(viewlist.at(i)->mac == pdata->net.macAddr)
                                {
                                    Equipment* temp = viewlist.at(i)->GetSlave();
                                    temp->setEquQTcpSocket(tcpClient);
                                    temp->EquInit(*pdata);
                                    break;
                                }
                            }

                        }
                        else
                        {
                            //如果列表中不存在则询问是否加入
                            bool is =myHelper::ShowMessageBoxQuesion("是否加入"+pdata->name+pdata->net.macAddr);
                            if(is)
                            {
                                //将控制器加入列表中

                                isremote = true;//说明控制器是客户端
                                RegisterSlaves(searchedAll);
                                searchedAll.clear();

                            }
                        }

                        //tcpClient=NULL;
                    }
                    else
                    {
                        delete pdata;
                    }

                }
                func=0;//表示数据处理一次完毕
            }
            else
            {
                //接收数据不完整
                break;
            }

        }

    }
    //弹出窗口询问是否加入些控制器


    //emit SearchedEqument(&viewlist,&searchedAll);

}
void frmMain::slotShowSetCjcParaDialog()
{
    if(!checkPermission(guestPermission.permitEditFangHuCangPar))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许修改工作参数！");
        return;
    }
    if(currentSlave)
    {
       if(currentSlave->GetEquAll()->slaveStatus.connectStatus == 1)
       {
           FrmEditCJCWorkPara *frmEditCjcWorkPara = new FrmEditCJCWorkPara();
           frmEditCjcWorkPara->initPara(currentSlave->GetEquAll()->fangHuCang,0);
           connect(frmEditCjcWorkPara,SIGNAL(sigWriteCjcWorkPara(paraFangHuCang*)),currentSlave,SLOT(slotSetWorkPara(paraFangHuCang*)));
           frmEditCjcWorkPara->setModal(true);
           frmEditCjcWorkPara->show();
       }
       else
       {
           myHelper::ShowMessageBoxInfo("设备未连接！");
       }
    }
    else
    {
        myHelper::ShowMessageBoxInfo("请首先选择设备！");
    }
}
//添加钥匙
void frmMain::slotbtnAddKey()
{
    //读取控制器中的用户Key
    currentSlave->slotReadKey();
    //设置控制器进入添加用户状态
    currentSlave->slotSetAddKeyStatus();
    //等待读取Key完成
    while(currentSlave->equParaData.readedKey)
    {
        QCoreApplication::processEvents();
    }
    //显示用户列表
    FrmSlaveUserList * frmSlaveKeyList = new FrmSlaveUserList();
    connect(currentSlave,SIGNAL(sigAddKey(QByteArray)),frmSlaveKeyList,SLOT(slotAddKey(QByteArray)));
    connect(frmSlaveKeyList,SIGNAL(sigReturnNormalStatus()),this,SLOT(slotSetJCJSlaveReturnNormalStatus()));
    frmSlaveKeyList->setModel(0);
    frmSlaveKeyList->setModal(true);//半模态调用对话框
    frmSlaveKeyList->DisplayKeyList(currentSlave->equParaData.keyList);
    frmSlaveKeyList->show();
}
//删除钥匙
void frmMain::slotbtnDeleteKey()
{
    //读取控制器中的用户Key

    currentSlave->slotReadKey();
    //等待读取Key完成
    while(currentSlave->equParaData.readedKey)
    {
        QCoreApplication::processEvents();
    }
    //显示用户列表
    FrmSlaveUserList * frmSlaveKeyList = new FrmSlaveUserList();
    frmSlaveKeyList->setModel(1);
    frmSlaveKeyList->setModal(true);//半模态调用对话框
    frmSlaveKeyList->DisplayKeyList(currentSlave->equParaData.keyList);
    connect(frmSlaveKeyList,SIGNAL(sigDeleteKeyId(int)),currentSlave,SLOT(slotDeleteKey(int)));
    connect(frmSlaveKeyList,SIGNAL(sigReturnNormalStatus()),this,SLOT(slotSetJCJSlaveReturnNormalStatus()));
    frmSlaveKeyList->show();

}

//更换管理员
void frmMain::slotbtnChangeAdminKey()
{
    if(!checkPermission(guestPermission.permitChangeAdmin))
    {
        myHelper::ShowMessageBoxInfo("操作员不允许更换管理员！");
        InsertOneSystemLogAndShow("操作员不允许更换管理员！");
        return;
    }
    //读取控制器中的主钥匙
    currentSlave->slotReadKey();
    //等待读取Key完成
    while(currentSlave->equParaData.readedKey)
    {
        QCoreApplication::processEvents();
    }
    //进入增加主钥匙状态
    currentSlave->slotSetAddMainKeyStatus();
    //显示用户列表
    FrmSlaveUserList * frmSlaveKeyList = new FrmSlaveUserList();
    connect(frmSlaveKeyList,SIGNAL(sigReturnNormalStatus()),this,SLOT(slotSetJCJSlaveReturnNormalStatus()));
    frmSlaveKeyList->setModal(true);//半模态调用对话框
    frmSlaveKeyList->show();
}
//设置控制器返回正常状态
void frmMain::slotSetJCJSlaveReturnNormalStatus()
{
    currentSlave->slotReturnNormalStatus();
}
