#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QMap>
#include <../datastruct.h>

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    void addChild(TreeItem * child);
    void clearChildItems(void);
    void clearParentItem(void);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parent();
    QList<TreeItem*> GetChildItems(void);
private:
    QList<TreeItem*> childItems;
    QList<QVariant> itemData;
    TreeItem *parentItem;

};

#endif // TREEITEM_H
