#ifndef FRMOPERATEPARENTINTREE_H
#define FRMOPERATEPARENTINTREE_H

#include <QDialog>
#include <QTcpSocket>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"
#include <QtSql>
namespace Ui {
class frmOperateParentInTree;
}

class frmOperateParentInTree : public QDialog
{
    Q_OBJECT

public:
    explicit frmOperateParentInTree(QWidget *parent = 0);
    ~frmOperateParentInTree();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::frmOperateParentInTree *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlTableModel *tableModel;
    QString qName;
    void InitStyle();
    bool changed;
signals:
    void sigParentInTreeChanged(bool);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnAddNewParentInTree();//添加区域
    void btnCancel();
    void btnDelete();
    void btnSave();
    void btnEdit();
public slots:
    void setTreeViewModel(QSqlTableModel * model);

};

#endif // FRMOPERATEPARENTINTREE_H
