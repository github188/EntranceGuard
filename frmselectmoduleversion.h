#ifndef FRMSELECTMODULEVERSION_H
#define FRMSELECTMODULEVERSION_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"
#include <QtSql>
#include "../DATA/versioninfo.h"
#include "../frmmodulepara.h"

namespace Ui {
class FrmSelectModuleVersion;
}

class FrmSelectModuleVersion : public QDialog
{
    Q_OBJECT

public:
    explicit FrmSelectModuleVersion(QWidget *parent = 0);
    ~FrmSelectModuleVersion();
    void setVoiceModel(QSqlTableModel *model);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmSelectModuleVersion *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlTableModel *voiceModel;
    void InitStyle();
signals:
    void sigSaveModule(QString,paraData*);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void on_btnOk_clicked();
    void btnCancel();
    void slotSaveModule(QString name,paraData* para);
public slots:
    void slotSetVersionInfoList(QList<VersionInfo*> list);
};

#endif // FRMSELECTMODULEVERSION_H
