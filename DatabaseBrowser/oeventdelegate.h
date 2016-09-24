#ifndef OEVENTDELEGATE_H
#define OEVENTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QSortFilterProxyModel>

#include "oeventmodel.h"

class OEventDelegate : public QStyledItemDelegate
{
    QSortFilterProxyModel* m;
    int colColor;

public:
    OEventDelegate(QSortFilterProxyModel* model, int ColColor = 3);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // COLORROWDELEGATE_H
