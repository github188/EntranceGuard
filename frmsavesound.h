#ifndef FRMSAVESOUND_H
#define FRMSAVESOUND_H

#include <QDialog>

namespace Ui {
class FrmSaveSound;
}

class FrmSaveSound : public QDialog
{
    Q_OBJECT

public:
    explicit FrmSaveSound(QWidget *parent = 0);
    ~FrmSaveSound();
    void ClearText();
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnOk_clicked();
public slots:
    //void serParentModel(TreeViewItem * model);

private:
    Ui::FrmSaveSound *ui;

    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下
signals:
    void sigoperate(QString name);
};

#endif // FRMSAVESOUND_H
