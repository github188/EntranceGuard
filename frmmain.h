#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QDialog>
#include <QGridLayout>
#include <QMenuBar>
#include "frmsearchequipment.h"
#include "frmaddslave.h"
#include "Equipment/equipment.h"
#include "../TreeViewClass/treeitem.h"
#include "../TreeViewClass/treemodel.h"
#include "../data/treeviewitem.h"
#include <QtSql>
#include "selectparentintree.h"
#include "frmoperateparentintree.h"
#include "../ListViewClass/listmodel.h"
#include "frmeditparamater.h"
#include "frmeditzhaomingpara.h"
#include "frmmanagervoice.h"
#include "frmsavesound.h"
#include "frmreadsoundprocess.h"
#include "frmselectsound.h"
#include "frmmanagermodule.h"
#include "frmusemodule.h"
#include "./frmmodulepara.h"
#include "../DATA/paramodule.h"
#include "./frmalarmbox.h"
#include "./frmeditalarmpara.h"
#include <QKeyEvent>
#include "./User/userinfo.h"
#include "../frmedituserpwd.h"
#include "../frmsoftwareinfo.h"
#include "../frmguestpermissionconfig.h"
#include "./DATA/voiceinfobank.h"
#include "./User/userpermission.h"
#include "./DATA/systemlogitem.h"
#include "./DATA/fanghucanglogitem.h"
#include "./DATA/slaveversion.h"
#include "../frmsearchslavelog.h"
#include "../frmsearchsystemlog.h"
#include "../TCPIP/mytcpserver.h"
#include "../frmeditcjcworkpara.h"
#include "../frmslaveuserlist.h"
#include "./DATA/versioninfo.h"
#define MaxConnectSlave (500)

namespace Ui {
class frmMain;

}

namespace Phonon{class MediaObject;}
class frmMain : public QDialog
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();
    void ReInitStyle();
    UserInfo currentUser;//当前用户
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
public slots:
    void SearchEqument();
    void slotConnectToSlave(QString ip);
    void CreatNewSlave(paraData prData);
    void updateParaDisplay(Equipment *temp);
    void slotParentInTreeDataChanged(bool changed);
    void slotTreeParentChanged(QModelIndex index1,QModelIndex index2);
    void sloteditedName(QString name);
    void slotUpdateDatabase(Equipment *slave);
    void slotShowOpenDoorMsg(quint8 val);
    void slotShowCloseLockMsg(quint8 val, quint8 result);
    void slotShowOpenLockMsg(quint8 val);
    void slotupdateSlaveTimeDisplay(paraData *pdata);
    void slotupdateSlaveStatusDisplay(paraData *pdata);
    void slotupdateSlaveParaDisplay(paraData *pdata);
    void slotupdateSlaveZhaoMingParaDisplay(paraData *pdata);
    void slotupdateSlaveFangHuCangParaDisplay(paraData *pdata);
    void slotupdateSlaveLedParaDisplay(paraData *pdata);
    void slotupdateSlaveVoiceParaDisplay(paraData *pdata);
    void slotupdateSlaveAlarmParaDisplay(paraData *pdata);
    void slotShowSoundSaveWindow(paraData *temp);
    void slotShowReadSoundProcess(quint64 size);
    void slotShowUseModuleProcess(quint64 size);
    void slotDeleteVoice(QString name);
    void slotAddVoice(QString filePath, QString name);
    void slotShowAddVoiceWindow();
    void slotBtnPlaySound(QString name);
    void slotShowAddModuleWindows();
    void slotShowEditModuleWindows(QModelIndex index);
    void slotSaveParaModule(paraModule *pModule);
    void slotSaveModulePara(paraModule * tempModule);
    void slotShowEditZhaoMingParaWindow();
    void slotShowEditFangHuCangParaWindow();
    void slotChangeModuleSound(quint8 id);
    void slotUpdateModuleSoundName(paraData* pdata);
    void slotDeleteModule(QModelIndex index);
    void slotUseModule(QModelIndex index);
    void slotUseModuleSound(paraData *temp,quint8 id);
    void slotShowWarningWindowAndPlaySound(Equipment * slave);
    void slotSetAlarmPara(paraAlarm* alarmPara);
    void slotShowSetCjcParaDialog();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void searchEqu();
    void addEqu();
    void readPacketage();
    void RegisterSlaves(QList<paraData *> registerList);
    void slotRegisterSlave(paraData *);
    void slotShowEditParaDialog();
    void slotShowEditZhaoMingParaDialog();
    void expanded(QModelIndex index);
    void slotchangeSlaveDisplay(QModelIndex index);
    void slotchangeSlaveDisplayFromListView(QModelIndex index);
    void slotTreeViewCustomContextMenuRequested(const QPoint &pos);
    void slotSelectParentIndex(QString name);
    void slotdeleteItem(void);
    void DeleteParentInTree(QString name);
    void slotEditItemMessageBox();
    void slotAddNewParentInTree();
    void slotmoveToArea();
    void btnWtiteNowTime();
    void slotOpenDoor();
    void slotEmitSearchDone();
    void slotShowReadMsgProcess();
    void slotInsertMsgToLog(QList<SlaveMsg *> list);
    //显示系统日志
    void slotShowSystemLog(QList<SystemLogItem *> list);
    //插入一条系统日志
    void slotInsertItemSystemLog(SystemLogItem * item);
    //显示防护舱日志
    void slotShowFangHuCangLog(QList<FangHuCangLogItem *> list);
    //插入一条防护舱日志
    void slotInsertItemFangHuCangLog(FangHuCangLogItem * item);

    void slotbtnLed0TextChange();
    void slotbtnSetLed0TextOk();
    void slotbtnLed1TextChange();
    void slotbtnSetLed1TextOk();
    void slotbtnLed2TextChange();
    void slotbtnSetLed2TextOk();
    void on_hsVoice_valueChanged(int value);
    void slotBtnSetSoundVolume();
    void slotBtnVoiceManager();
    void slotBtnChangeHySound();
    void slotBtnChangesyzSound();
    void slotBtnChangeWhzSound();
    void slotBtnChangeTimeoutWarnSound();
    void slotBtnChangeTimeoutTiShiSound();
    void slotBtnChangeOutDoorSound();
    void slotBtnChangeOpenDoorSound();
    void slotBtnChangeFailLockSound();
    void slotBtnChangeLaDoorSound();
    void slotBtnReadHySound();
    void slotBtnReadSyzSound();
    void slotBtnReadWhzSound();
    void slotBtnReadTimeoutWarnSound();
    void slotBtnReadTimeoutTiShiSound();
    void slotBtnReadFailLockSound();
    void slotBtnReadOpenDoorSound();
    void slotBtnReadOutDoorSound();
    void slotBtnReadLaDoorSound();
    void slotBtnPlayHySound();
    void slotBtnPlaySyzSound();
    void slotBtnPlayWhzSound();
    void slotBtnPlayTimeoutWarnSound();
    void slotBtnPlayTimeoutTiShiSound();
    void slotBtnPlayOutDoorSound();
    void slotBtnPlayOpenDoorSound();
    void slotBtnPlayFailLockSound();
    void slotBtnPlayLaDoorSound();
    void slotShowManagerModuleWindows();
    void slotShowUseModuleWindows();
    void on_btnSetAlarmPara_clicked();
    void slotSelectionChanged(QItemSelection x,QItemSelection y);
    void connectIconChanged();//更新显示连接中状态时的gif图片
    //批量修改区域内的照明参数
    void slotEditAreaSlaveZhaoMingPara();
    void slotSaveAreaSlaveZhaoMingPara(paraZhaoMing * zmpara);
    //批量修改区域内的工作参数
    void slotEditAreaSlaveWorkPara();
    void slotSaveAreaSlaveWorkPara(paraFangHuCang * fhcpara);
    //显示修改密码的页面
    void slotShowEditUserPwdWindow();
    //显示联系方式页面
    void slotShowContactWindow();
    //修改密码
    void slotUpdateUserPwd();
    //修改操作员权限窗口
    void slotShowEditUserPermissionWindow();
    void slotOpenHelpFile();
    //修改操作员权限
    void slotEditUserPermission(UserPermission *usr);

    void slotEnableOrDisableSlave();

    void slotRemoteCloseLock();

    void slotReadSlaveMsg();

    void slotShowSearchSlaveLogWindow();

    void slotShowSearchSystemLogWindow();

    void slotUpdateTSlave(Equipment *slave);
    void slotAnalysisData();
    void ClientConnect(int handle,QTcpSocket *client);
    void sendTcpPacketage(quint8 func);
    void receiveData();
    void slotAnalysisTcpData();
    void slotbtnAddKey();
    void slotbtnDeleteKey();
    void slotbtnChangeAdminKey();
    void slotSetJCJSlaveReturnNormalStatus();
    void slotSetUserParaEnable();
signals:
    void SearchedEqument(QList<TreeViewItem*> *registeredList,QList<paraData*> *paradataList);
    void AddedEqument();
    void sigSendParentTbModel(QSqlTableModel *);
    void sigSendVoiceTbModel(QSqlTableModel *);
    void sigSendModuleTbModel(QList<SlaveVersion *> list);
    void sigSendModuleTbModel(QList<SlaveVersion *>,QList<paraModule*> list);
    void sigConnectedToSlave(bool,paraData*);
    void sigWriteNowTime(quint8 year, quint8 month, quint8 day, quint8 hour, quint8 minute, quint8 second, quint8 week);
    void sigEditFangHuCangPara(paraFangHuCang fangHuCangPara,quint8 mod);
    void sigZhaoMingPara(paraZhaoMing tempPara,quint8 mod);
    void sigOpenDoor(bool enable);
    void sigSetLedText(quint8 dula,QString text);
    void sigSetSlaveVolume(quint8 volume);
    void sigChangeSoundIdAndName(quint8 id,QString name);//写入语音内容信号
    void sigChangeSoundIdAndMd5(quint8 id,QString name);//写入Md5内容信号
    void sigChangeSound(quint8 id,QString fileName,QString md5);
    void sigReadHySound();
    void sigReadSyzSound();
    void sigReadWhzSound();
    void sigReadTimeoutWarnSound();
    void sigReadTimeoutTiShiSound();
    void sigReadOutDoorSound();
    void sigReadOpenDoorSound();
    void sigReadFailLockSound();
    void sigReadLaDoorSound();
    void sigPlaySlaveSound(quint8 id);
    void sigChangeModuleSound(quint8 id,QString md5);
    void sigUseModule(paraData * tempPara);
    void sigEanbleOrDisableSlave();
    void sigRemoteCloseLock();
    void sigReadSlaveMsg();
    void sigAddVoiceEnd(int end);
    void sigAnalysisData();
private:
    Ui::frmMain *ui;
    UserPermission guestPermission;//操作员账号权限
    static QString ConnectVersion;//软件支持的版本号
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    quint8 windowStatus;//1-防护舱状态 2－加钞间状态 作用：防止窗口多次刷新
    CCycleBuffer* comBuffer;
    QPixmap currFrame;
    QRect location;
    QMenu *treeViewContextMenu;//treeView右键菜单
    QMenuBar* menuBar;
    QToolBar *toolBar;
    QAction* actionSearch;
    QAction* actionAdd;
    QAction * enableUserPara;//使能用户参数
    //添加修改操作员软件的菜单
    QAction *editUserPermission;
    QString editedName;
    QString addSlaveIp;
    QMovie *movie;
    QIcon iconConnected;
    QIcon iconDisConnected;
    QIcon iconConnecting;
    //加钞间状态图标
    QString iconCjcWorkNormalState;//工作状态－正常
    QString iconCjcWorkIsGuardingState;//工作状态－正常 有人值守
    QString iconCjcWorkOperateMainCardState;//工作状态－主卡添加，删除
    QString iconCjcWorkEditAdminState;//工作状态－修改管理员

    QString iconCjcAlarmSetGuardState;//报警状态－设防
    QString iconCjcAlarmWaitSetGuardState;//报警状态－等待设防
    QString iconCjcAlarmResetGuardState;//报警状态－撤防

    QString iconCjcAlarmIllegalOpenDoorEnable;//非法开门报警状态－报警
    QString iconCjcAlarmIllegalOpenDoorDisable;//非法开门报警状态－正常

    QString iconCjcAlarmIllegalInEnable;//非法入侵报警状态－报警
    QString iconCjcAlarmIllegalInDisable;//非法入侵报警状态－正常
    quint8 userParaEnable;
    //
    QString gateOpen;
    QString gateClose;
    QString statusNull;
    QString openLock;
    QString closeLock;
    QString doorStatusNull;
    QString slaveTime;
    QString fanClose;
    QString warning;
    QMovie * usingGif;
    QMovie * connectingGif;
    QString notUsingIcon;
    QString zhaoMingOpen;
    QString zhaoMingClose;
    QString dengXiangOpen;
    QString dengXiangClose;
    QString qieGeAlarm;
    QString qieGeNormal;
    QString baoJingDisable;
    QString btnAlarm;
    QString btnNormal;
    QString zhenDongAlarm;
    QString zhenDongNormal;
    QString yanWuAlarm;
    QString yanWuNormal;
    QString tempAlarm;
    QString tempNormal;
    QString boLiAlarm;
    QString boLiNormal;
    QString shuiQinAlarm;
    QString shuiQinNormal;
    QString fangHuCangUse;
    QString fangHuCangNotUse;
    Equipment * currentSlave;
    QList<Equipment*> classList;//所有设备列表
    QList<paraModule*> moduleList;
    QList<SlaveVersion*> versionList;
    QList<VersionInfo*> versioninfoList;
    QList<SystemLogItem*> systemLogList;
    QList<FangHuCangLogItem*> fangHuCangLogList;

    frmSearchEquipment *searchEquForm;
    frmAddSlave *addEquForm;
    SelectParentInTree *frmSelectParentInTree;
    QString selectedParentName;
    //frmOperateParentInTree * frmOperateParent;
    FrmOperateParentMsgBox *frmOperateItemBox;
    //frmEditParamater *frmEditSlavePara;
    //frmEditZhaoMingPara *frmEditZhaoMing;
    FrmManagerVoice * frmVoiceManager;
    FrmSaveSound * frmSaveSound;
    FrmReadSoundProcess * frmReadSoundProcess;
    //FrmSelectSound * frmSelectSound;
    //FrmAddSound * frmAddSound;
    FrmManagerModule *frmManagerModel;
    FrmUseModule *frmUseModule;
    //FrmModulePara *frmModulePara;
    frmAlarmBox * frmAlarm;
    //frmEditAlarmPara * frmEditAlarm;
    FrmEditUserPwd *frmEditUserPwd;
    frmSoftwareInfo *frmInfo;
    frmGuestPermissionConfig *frmEditUserPermission;
    //数据库
    QSqlDatabase database;
    QSqlQuery *query;
    QSqlQuery *systemLogquery;
    QSqlQuery *slaveLogquery;
    //QString create_sql;
    QString select_indexParent_sql;
    QString select_macParent_sql;
    QString select_TSlave_sql;
    QString select_Parent_sql;
    QString select_TParamater_sql;
    QString select_TVoice_sql;
    QString select_Module_sql;
    QString select_TMparamater_sql;
    QString select_TNetWork_sql;
    QString select_TUser_sql;

    QString insert_TZone_sql;
    QString insert_TSlave_sql;
    QString insert_TParamater_sql;
    QString insert_TBelong_sql;
    QString insert_TVoice_sql;
    QString insert_TModule_sql;
    QString insert_TMparamater_sql;
    QString insert_TSystemLog_sql;
    QString insert_TNetWork_sql;

    QString delete_TSlave_sql;
    QString delete_TZone_sql;
    QString delete_TBelong_sql;
    QString delete_TParamater_sql;
    QString delete_TVoice_sql;
    QString delete_TModule_sql;
    QString delete_TMparamater_sql;
    QString delete_TNetWork_sql;

    QString update_TSlave_index_sql;
    QString update_TSlave_sql;
    QString update_TZone_index_sql;
    QString update_TZone_name_sql;
    QString update_TBelong_ZName_sql;
    QString update_TPara_sql;
    QString update_TVoice_sql;
    QString update_TModule_sql;
    QString update_TMparamater_sql;
    QString update_TPuser_sql;
    QString update_TNetWork_sql;
    //Udp/Tcp通讯
    bool isremote;//添加控制器时判断是否是客户端
    QList<QTcpSocket *> tcpClientList;
    QTimer *searchTimeOut;
    QTimer *searchTime;
    QUdpSocket *udpClient;
    QTcpSocket *tcpClient;
    myTcpServer *tcpServer;
    QList<paraData*> searchedAll;
    QList<paraData*> searchedNoRegister;
    TreeModel *model;
    QModelIndex parentIndex;
    ListModel * slaveListModel;
    QList<TreeViewItem*> viewlist;//树结构中显示的列表
    Phonon::MediaObject *mMediaObj;
    Phonon::AudioOutput *audioOutput;
    //Phonon::SeekSlider *seekSlider;
    //Phonon::MediaSource *source[4];
    //
    QItemSelectionModel *selectionModel;

    QItemSelectionModel *listViewSelectModel;

    QSqlTableModel *treeViewParentTbModel;
    QSqlTableModel *voiceModel;
    QSqlTableModel *moduleModel;
    void sendUdpPacketage(QHostAddress address,quint8 func,quint16 port);
    void InitStyle();

    void InitForm();
    void updateSlaveListDisplay();
    void changeStockedPageDisplay(int index);
    //插入新设备到数据库
    void InsertEquipment(Equipment *temp);
    //插入数据库设备表
    void Insert_TSlave_Sql(Equipment *temp);
    //插入数据库树形父名称表
    void Insert_TZone_Sql(QString name, int areaIndex);
    //插入设备参数表
    void Insert_TParamater_Sql(Equipment *temp);
    //插入数据库父表
    void Insert_TBelong_Sql(Equipment *temp);
    //插入设备网络参数
    void Insert_TNetWork_Sql(Equipment *temp);
    void Insert_TVoice_Sql(QString name,QString fileName,QString md5);
    void DeleteSlave(QString mac);
    void delete_TSlave_Sql(QString mac);
    void delete_TBelong_Sql(QString mac);
    void delete_TZone_Sql(QString name);
    void delete_TParamater_Sql(QString mac);
    void delete_TVoice_Sql(QString name);
    void delete_TModule_Sql(QString name);
    void delete_TMparamater_Sql(QString name);
    void delete_TNetWork_Sql(QString mac);
    void Update_TParamater_Sql(Equipment *slave);
    QList<paraData*> removeRegisteredSlave(QList<paraData*> list);
    bool checkExist(QString table,QString item,QString value);
    bool checkExist(QString table,QString item,int value);
    void getParentTreeModel();
    QList<TreeViewItem*> getSortedList(QList<TreeViewItem*> list);
    void slaveChangeConnectToForm(Equipment* tempSlave, QList<TreeViewItem *> *tempList);
    void Update_TSlave_index_sql(QString mac,int index);
    void Update_TZone_index_sql(QString name,int index);
    void Update_TZone_name_sql(QString name,int index);
    void Update_TBelong_ZName_sql(QString mac,QString name);
    void Update_TVoice_Sql(QString name,QString md5,QString fileName);
    void refreshListView(QModelIndex index);
    void clearFormParaDisplay(void);
    void clearFormStatusDisplay(void);
    void slaveConnectSignal(Equipment * slave);
    void slaveDisConnectSignal(Equipment * slave);
    void readVoiceData();
    void getSlaveSoundName(paraData *pdata);
    void UpdateVoice(QString name,QString md5,QString fileName);
    void Select_TVoice_FileName_Md5(QString name,QString &fileName,QString &md5);
    void Select_TVoice_FileName_Name(QString &name,QString &fileName,QString md5);
    void Select_TVoice_FileName(QString &fileName,QString md5);
    void readModuleData();
    void Insert_TMparamater_Sql(QString name, paraData *pdat);
    void Update_TMparamater_Sql(QString name, paraData *pdat);
    void Update_TModule_sql(QString name);
    void Insert_TModule_Sql(paraModule *pModule);
    void addModuleVoice(QString filePath,QString name);
    void readSettings();
    void writeSettings();
    void changeVoice(quint8 id);
    bool checkPermission(bool permit);//查询权限
    void InsertOneSystemLogAndShow(QString item);
    void InsetItemSystemLogToDB(SystemLogItem * item);
    void InsetItemSlaveLogToDB(FangHuCangLogItem * item);
    void Update_TSlave_sql(Equipment *slave);
    void connectToUpdateSlaveToDB(Equipment *slave);
    void disconnectToUpdateSlaveToDB(Equipment *slave);
    //添加加钞间后新的函数
    void changeJCJText();
    void changeFHCText();
protected:
    void keyPressEvent(QKeyEvent *);
};

#endif // FRMMAIN_H
