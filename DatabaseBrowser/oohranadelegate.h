#ifndef OOHRANADELEGATE_H
#define OOHRANADELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QSortFilterProxyModel>

#include "oohranamodel.h"

class OOhranaDelegate : public QStyledItemDelegate
{
    QSortFilterProxyModel* m;
    int colColor;

public:
    OOhranaDelegate(QSortFilterProxyModel* model, int ColColor = 3);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // OOHRANADELEGATE_H
