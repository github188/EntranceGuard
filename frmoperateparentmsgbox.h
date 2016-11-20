#ifndef FRMOPERATEPARENTMSGBOX_H
#define FRMOPERATEPARENTMSGBOX_H

#include <QDialog>
#include "DATA/treeviewitem.h"

namespace Ui {
class FrmOperateParentMsgBox;
}

class FrmOperateParentMsgBox : public QDialog
{
    Q_OBJECT

public:
    explicit FrmOperateParentMsgBox(QWidget *parent = 0);
    ~FrmOperateParentMsgBox();
    void SetMessage(const QString &msg, int type);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnOk_clicked();
public slots:
    //void serParentModel(TreeViewItem * model);

private:
    Ui::FrmOperateParentMsgBox *ui;

    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下
signals:
    void sigoperate(QString name);

};

#endif // FRMOPERATEPARENTMSGBOX_H
