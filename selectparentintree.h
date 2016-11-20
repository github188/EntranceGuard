#ifndef SELECTPARENTINTREE_H
#define SELECTPARENTINTREE_H

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
class SelectParentInTree;
}

class SelectParentInTree : public QDialog
{
    Q_OBJECT

public:
    explicit SelectParentInTree(QWidget *parent = 0);
    ~SelectParentInTree();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::SelectParentInTree *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    QSqlTableModel *tableModel;
    QString qName;
    void InitStyle();
signals:
    void sigSelected(QString);
    void sigAddNewParentInTree();
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnAddParentInTree();//添加区域
    void btnCancel();
    void btnSelect();
public slots:
    void setTreeViewModel(QSqlTableModel * model);
    void getSelectItem(QModelIndex index);

};

#endif // SELECTPARENTINTREE_H
