#ifndef FRMADDSOUND_H
#define FRMADDSOUND_H

#include <QDialog>
#include "../iconhelper.h"
#include "../myhelper.h"
#include "../TCPIP/myudpsocket.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"
#include "QsLog/QsLogLevel.h"
#include "../datastruct.h"
#include <QtSql>

namespace Ui {
class FrmAddSound;
}

class FrmAddSound : public QDialog
{
    Q_OBJECT

public:
    explicit FrmAddSound(QWidget *parent = 0);
    ~FrmAddSound();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::FrmAddSound *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;
    void InitStyle();
    QString filePath;
    QString fileName;
signals:
    void sigAddVoice(QString filePath,QString name);
private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void btnFile();
    void btnOK();
};

#endif // FRMADDSOUND_H
