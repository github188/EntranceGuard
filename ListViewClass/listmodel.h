#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "../data/treeviewitem.h"

class TreeItem;

class ListModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ListModel(QObject *parent = 0);
    ~ListModel();
    QVariant data(const QModelIndex &index, int role) const;
    Equipment* GetSlave(const QModelIndex &index);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void setData(QList<TreeViewItem *> treeviewlist);
    QList<TreeViewItem *> GetData(void);
    TreeItem*  GetTreeItem(const QModelIndex &index);
    void update();
private:
    void setupModelData(TreeItem *parent);
    TreeItem *item(TreeItem *item, TreeViewItem *treeViewItem);
    TreeItem *rootItem;
    QList<TreeViewItem*> treeViewItemList;
    //QList<QIcon> iconList;
public slots:
    void slotDataChanged(int index, int parentIndex);

};

#endif // LISTMODEL_H
