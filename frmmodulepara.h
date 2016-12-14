#ifndef FRMMODULEPARA_H
#define FRMMODULEPARA_H

#include <QDialog>
#include "./DATA/paradata.h"
#include "../frmeditalarmpara.h"
#include "../frmeditparamater.h"
#include "../frmeditzhaomingpara.h"
#include "../frmselectsound.h"
#include "../frmeditcjcworkpara.h"
#include "../DATA/paramodule.h"
namespace Ui {
class FrmModulePara;
}

class FrmModulePara : public QDialog
{
    Q_OBJECT

public:
    explicit FrmModulePara(QWidget *parent = 0);
    ~FrmModulePara();
    void SetTitleText(QString title);
    void SetPara(QString name, paraModule * pModule, quint8 mod);
    void setModule(QString name,paraModule * pModule,quint8 mod);
    void setVoiceModel(QSqlTableModel *model);
    void setModel(quint8 mod);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmModulePara *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    paraData* pdat;
    QString editedName;
    QSqlTableModel *voiceModel;
    quint8 model;
    paraModule * pModule;
    //frmEditParamater *frmEditSlavePara;
    //frmEditZhaoMingPara *frmEditZhaoMing;
    //frmEditAlarmPara *frmeditalarmpara;
    //FrmSelectSound * frmSelectSound;
    void InitStyle();
    void DisplayZhaoMingPara();
    void DisplayFangHuCangPara();
    void DisplayAlarmPara();
    void DisplaySoundName();
    void findfileName(QString &fileName,QString &md5);
    void changeVoice(quint8 id);
signals:
    void sigEditFangHuCangPara(paraFangHuCang para,quint8 mod);
    void sigEditZhaoMingPara(paraZhaoMing para,quint8 mod);
    void sigShowEditFangHuCangParaWindow();
    void sigShowEditZhaoMingParaWindow();
    void sigEditVoicePara();
    void sigChangModuleSound(quint8 id);
    void sigUpdateSoundName(paraData *para);
    void sigSaveModulePara(paraModule *pModule);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnOK();
    void btnCancel();
    void btnSetZhaoMingPara();
    void btnSetFangHuCangPara();
    void slotBtnChangeHySound();
    void slotBtnChangesyzSound();
    void slotBtnChangeWhzSound();
    void slotBtnChangeTimeoutWarnSound();
    void slotBtnChangeTimeoutTiShiSound();
    void slotBtnChangeOutDoorSound();
    void slotBtnChangeOpenDoorSound();
    void slotBtnChangeFailLockSound();
    void slotBtnChangeLaDoorSound();
    void on_hsVoice_valueChanged(int value);
    void slotbtnLed0TextChange();
    void slotbtnLed1TextChange();
    void slotbtnLed2TextChange();
    void slotbtnSetLed0TextOk();
    void slotbtnSetLed1TextOk();
    void slotbtnSetLed2TextOk();
    void slotbtnSetAlarmPara();
    void slotRefreshUI(paraAlarm *par);
    void slotShowSetCjcParaDialog();
    void slotSetJCJPara(paraFangHuCang * data);
public slots:
    void slotSetZhaoMingPara(paraZhaoMing * data);
    void slotSetFangHuCangPara(paraFangHuCang *data);
    void slotSetSound(quint8 id,QString md5);
    void sloteditedName(QString name);


};

#endif // FRMMODULEPARA_H
