#include "osearchdelegate.h"

OSearchDelegate::OSearchDelegate(QSortFilterProxyModel *model, int ColColor)
{
    m = model;
    colColor = ColColor;
}

void OSearchDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int row = index.row();
    QStyleOptionViewItem opt(option);

    if (m->index(row, colColor).data().toString() == "Установлено на охрану")
        opt.palette.setColor(QPalette::Text, QColor(0, 150, 0));
    else if (m->index(row, colColor).data().toString() == "Снято с охраны")
        opt.palette.setColor(QPalette::Text, QColor(150, 0, 0));
    else if (m->index(row, colColor).data().toString() == "Дверь закрыта")
        opt.palette.setColor(QPalette::Text, QColor(0, 0, 150));
    else if (m->index(row, colColor).data().toString() == "Дверь открыта")
        opt.palette.setColor(QPalette::Text, QColor(150, 0, 150));

    painter->setOpacity(0.9);

    QStyledItemDelegate::paint(painter, opt, index);
}
