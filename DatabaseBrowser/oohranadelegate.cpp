#include "oohranadelegate.h"

OOhranaDelegate::OOhranaDelegate(QSortFilterProxyModel *model, int ColColor)
{
    m = model;
    colColor = ColColor;
}

void OOhranaDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int row = index.row();
    QStyleOptionViewItem opt(option);

    if (m->index(row, colColor).data().toString() == "Установлено на охрану")
        opt.palette.setColor(QPalette::Text, QColor(0, 150, 0));
    else if (m->index(row, colColor).data().toString() == "Снято с охраны")
        opt.palette.setColor(QPalette::Text, QColor(150, 0, 0));

    painter->setOpacity(0.9);

    QStyledItemDelegate::paint(painter, opt, index);
}
