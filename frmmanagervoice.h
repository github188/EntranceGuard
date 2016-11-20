#ifndef FRMMANAGERVOICE_H
#define FRMMANAGERVOICE_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"
#include <QtSql>
#include "../frmaddsound.h"
namespace Ui {
class FrmManagerVoice;
}

class FrmManagerVoice : public QDialog
{
    Q_OBJECT

public:
    explicit FrmManagerVoice(QWidget *parent = 0);
    ~FrmManagerVoice();
    void DisableorEnableEdit(bool enable);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmManagerVoice *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlTableModel *tableModel;
    void InitStyle();
signals:
    void sigDeleteVoice(QString name);
    void sigAddVoicetoDatabase(QString path,QString name);
    void sigPlaySound(QString name);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnCancel();
    void btnDelete();
    void btnAdd();
    void btnPlaySound();
    void slotAddVoice(QString filePath,QString name);
public slots:
    void setTreeViewModel(QSqlTableModel * model);
    void slotShowAddVoiceEndWindow(int end);
};

#endif // FRMMANAGERVOICE_H
