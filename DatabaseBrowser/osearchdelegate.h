#ifndef OSEARCHDELEGATE_H
#define OSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QSortFilterProxyModel>

#include "osearchmodel.h"

class OSearchDelegate : public QStyledItemDelegate
{
    QSortFilterProxyModel* m;
    int colColor;

public:
    OSearchDelegate(QSortFilterProxyModel* model, int ColColor = 3);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // OSEARCHDELEGATE_H
