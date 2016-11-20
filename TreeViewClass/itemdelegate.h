#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QTreeView>

class ItemDelegate:public QStyledItemDelegate
{
  Q_OBJECT

public:
    ItemDelegate(QObject * parent=0);
    virtual ~ItemDelegate(){}
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void setView(QTreeView *treeView);
    void setIconSize(QSize size);
    void setExpandIcon(QString expandIconName);
    void setcollapseIcon(QString collapseIconName);

signals:
    void expanded(const QModelIndex &index);

private:
    QTreeView *m_treeView;
    int m_pixmapHeight;
    int m_pixmapWidth;
    QString m_expandIconName;
    QString m_collapseIconName;

};
#endif // ITEMDELEGATE_H
