#ifndef FRMMANAGERMODULE_H
#define FRMMANAGERMODULE_H

#include <QDialog>
#include <QModelIndex>
#include <QSqlTableModel>
#include "../DATA/paramodule.h"
#include "../frmmodulepara.h"
namespace Ui {
class FrmManagerModule;
}

class FrmManagerModule : public QDialog
{
    Q_OBJECT

public:
    explicit FrmManagerModule(QWidget *parent = 0);
    ~FrmManagerModule();
    void setVoiceModel(QSqlTableModel *model);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmManagerModule *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlTableModel *voiceModel;
    QSqlTableModel *tableModel;
    QList<paraModule*> moduleList;
    FrmModulePara *frmModulePara;
    void InitStyle();
signals:
    //添加设备信号
    void sigAddParaModule(void);
    void sigEditParaModule(QModelIndex index);
    void sigDelParaModule(QModelIndex index);
    void sigInParaModule(paraModule*);
    void sigSaveModulePara(QString name,paraData *data);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnDeleteModule();
    void btnAddModule();
    void btnCancel();
    void btnEditPara();
    void slotbtnIn();
    void slotbtnOut();
    void slotSaveModule(QString name,paraData *data);
public slots:
    void slotSetTableModel(QSqlTableModel * model,QList<paraModule*> list);
};

#endif // FRMMANAGERMODULE_H
