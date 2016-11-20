#include <QStringList>
#include "treeitem.h"

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    for(int i=0;i<childItems.count();i++)
    {
        if(item->data(0) == childItems.at(i)->data(0))
        {
            return;
        }
    }
    childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return childItems.value(row);
}
void TreeItem::addChild(TreeItem * child)
{
    childItems.append(child);
}
int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

int TreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
void TreeItem::clearChildItems(void)
{
    childItems.clear();
}
void TreeItem::clearParentItem(void)
{
    parentItem=NULL;
}
QList<TreeItem*> TreeItem::GetChildItems(void)
{
    return this->childItems;
}
