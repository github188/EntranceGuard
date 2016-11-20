/*
 * 项视图类
*/
#ifndef TREEVIEWITEM_H
#define TREEVIEWITEM_H
#include <QObject>
#include <qglobal.h>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
#include "../QsLog/QsLog.h"
#include "../QsLog/QsLogDest.h"
#include "../QsLog/QsLogLevel.h"
#include <QDateTime>
#include "../Equipment/equipment.h"
#include <QIcon>
class TreeViewItem :public QObject
{
    Q_OBJECT
public:
    TreeViewItem();
    ~TreeViewItem();
    int parentIndexInTree;
    int index;
    QString mac;
    QString text;
    QString tooltip;
    QIcon icon,icon1;
    QIcon getQicon(void);
    QIcon getListViewQicon(void);
    void setSlave(Equipment *equipment);
    Equipment* GetSlave(void);
private:
    QMovie *_movie;
    Equipment * slave;
    QPixmap currFrame;
public slots:
    void dataChanged(void);
signals:
    void sigDataChanged(int,int);

};

#endif // TREEVIEWITEM_H
