#include "listmodel.h"
#include "../TreeViewClass/treeitem.h"
#include <QStringList>
#include <QIcon>
#include <QDebug>

ListModel::ListModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    update();
}
void ListModel::update()
{
    rootItem=NULL;
    QList<QVariant> rootData;
    rootData << "门禁监控系统";
    rootItem = new TreeItem(rootData);
}
ListModel::~ListModel()
{
    delete rootItem;
}
void ListModel::setData(QList<TreeViewItem*> treeviewlist)
{
    update();
    treeViewItemList.clear();
    beginResetModel();
    treeViewItemList = treeviewlist;
    setupModelData(rootItem);
    for(int i=0;i<treeViewItemList.count();i++)
    {
        connect(treeViewItemList.at(i),SIGNAL(sigDataChanged(int,int)),this,SLOT(slotDataChanged(int,int)));
    }
    endResetModel();
}

int ListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    TreeViewItem *treeViewItem;
    for (int i = 0; i < treeViewItemList.count(); ++i)
    {
        treeViewItem = treeViewItemList.at(i);
        if (treeViewItem->mac == item->data(0))
        {
            break;
        }
    }

    if (role == Qt::DisplayRole)
    {
        return treeViewItem->text;
    }
    else if (role == Qt::DecorationRole)
    {
        return treeViewItem->getListViewQicon();

        //return QVariant();
    }
    else if (role == Qt::ToolTipRole)
    {
        return treeViewItem->text;;
    }
    else
    {
        return QVariant();
    }
}
TreeItem*  ListModel::GetTreeItem(const QModelIndex &index)
{
    TreeItem *retTreeItem=NULL;
    if (!index.isValid())
        return retTreeItem;

    retTreeItem = static_cast<TreeItem*>(index.internalPointer());

    return retTreeItem;

}
Equipment*  ListModel::GetSlave(const QModelIndex &index)
{
    Equipment *retSlave=NULL;
    if (!index.isValid())
        return retSlave;

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    TreeViewItem *treeViewItem;
    for (int i = 0; i < treeViewItemList.count(); ++i)
    {
        treeViewItem = treeViewItemList.at(i);
        if (treeViewItem->mac == item->data(0))
        {
            retSlave=treeViewItem->GetSlave();
            break;
        }
    }

    return retSlave;

}
Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex ListModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ListModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

TreeItem * ListModel::item(TreeItem* item, TreeViewItem* treeViewItem)
{
    TreeItem *treeItem = NULL;
    if (item == NULL)
    {
        return treeItem;
    }

    int parentId = treeViewItem->parentIndexInTree;

    if (item->data(1) == parentId)
    {
        treeItem = item;
    }
    else
    {
        for (int j = 0; j < item->childCount(); ++j)
        {
            TreeItem *childItem = item->child(j);
            TreeItem *item2 = this->item(childItem, treeViewItem);
            if (item2)
            {
                treeItem = item2;
                break;
            }
        }
    }

    return treeItem;
}
void ListModel::slotDataChanged(int index, int parentIndex)
{
    QModelIndex parentIndex1=this->index(parentIndex,0,QModelIndex());
    QModelIndex modelIndex=this->index(index,0,parentIndex1);
    emit dataChanged(modelIndex,modelIndex);

}
QList<TreeViewItem *> ListModel::GetData(void)
{
    return this->treeViewItemList;
}
void ListModel::setupModelData(TreeItem *parent)
{
    QList<TreeItem*> parents;
    parents.clear();
    parents << parent;

    for (int i = 0; i < treeViewItemList.count(); ++i)
    {
        TreeViewItem* treeViewItem = treeViewItemList.at(i);
        QList<QVariant> columnData;
        columnData << treeViewItem->mac;
        columnData << treeViewItem->index;
        columnData << treeViewItem->parentIndexInTree;
        parents.at(0)->appendChild(new TreeItem(columnData, parents.last()));

    }
}
