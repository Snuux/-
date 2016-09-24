#include "oeventdelegate.h"

OEventDelegate::OEventDelegate(QSortFilterProxyModel* model, int ColColor)
{
    m = model;
    colColor = ColColor;
}

void OEventDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int row = index.row();
    QStyleOptionViewItem opt(option);

    if (m->index(row, colColor).data().toString() == "Дверь закрыта")
        opt.palette.setColor(QPalette::Text, QColor(0, 150, 0));
    else if (m->index(row, colColor).data().toString() == "Дверь открыта")
        opt.palette.setColor(QPalette::Text, QColor(150, 0, 0));

    painter->setOpacity(0.9);

    QStyledItemDelegate::paint(painter, opt, index);
}
