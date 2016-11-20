#ifndef FRMSEARCHEQUIPMENT_H
#define FRMSEARCHEQUIPMENT_H

#include <QDialog>
#include <QTcpSocket>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"
namespace Ui {
class frmSearchEquipment;
}

class frmSearchEquipment : public QDialog
{
    Q_OBJECT

public:
    explicit frmSearchEquipment(QWidget *parent = 0);
    ~frmSearchEquipment();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmSearchEquipment *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QList<paraData*> *searchedList;
private:
    void InitStyle();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void pushButton_clicked();
    void RegisterSlaves();
    void btnCancel();
    void btnSelectAll();
    void btnSelectNull();
    void btnSelectChange();
public slots:
    void DisplayResult(QList<TreeViewItem *> *registeredList, QList<paraData *> *);
    void showSlaveList(QList<paraData*> *searchedList);
signals:
    void SearchEqument();
    void registerSlaves(QList<paraData*>);
    void sigShowSlaveList(QList<paraData*> *searchedList);
};

#endif // FRMSEARCHEQUIPMENT_H
