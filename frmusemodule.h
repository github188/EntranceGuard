#ifndef FRMUSEMODULE_H
#define FRMUSEMODULE_H

#include <QDialog>
#include <QTableView>
#include <QSqlTableModel>
#include "../DATA/slaveversion.h"
namespace Ui {
class FrmUseModule;
}

class FrmUseModule : public QDialog
{
    Q_OBJECT

public:
    explicit FrmUseModule(QWidget *parent = 0);
    ~FrmUseModule();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmUseModule *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlTableModel *tableModel;
    QList<SlaveVersion *> slaveVersionList;
    void InitStyle();
signals:
    //添加设备信号
    void sigSelectModule(QModelIndex index);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnOK();
    void btnCancel();
public slots:
    void slotSetTableModel(QList<SlaveVersion *> versionList);
};

#endif // FRMUSEMODULE_H
