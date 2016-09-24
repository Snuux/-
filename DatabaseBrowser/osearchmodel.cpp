#include "osearchmodel.h"

OSearchModel::OSearchModel()
{

}

int OSearchModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

int OSearchModel::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant OSearchModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
    const OEvent& ev = m_data[index.row()];

    if (role == Qt::BackgroundColorRole)
    {
        if (ev.getAct_num() == 1)
            return QVariant(QColor(Qt::red));
        else
            return QVariant(QColor(Qt::blue));
    }

    switch(index.column()){
    case 0: return ev.getId();
    case 1: return ev.getObj().getName();
    case 2: return ev.getObj().getPlace();
    case 3: return ev.getDate();
    case 4: return ev.getTime();
    case 5: return (ev.getSkip() ? "Есть" : "");
    case 6:
        if (ev.getType() == 1)
            return "Дверь";
        else if (ev.getType() == 2)
            return "Охрана";
        else if (ev.getType() == 3)
            return "Тревога";
        else return 0;
    case 7: return ev.getAct();
    default: return QVariant();
    }
}

QVariant OSearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    switch (section) {
    case 0: return "ID";
    case 1: return "Объект";
    case 2: return "Адрес";
    case 3: return "Дата";
    case 4: return "Время";
    case 5: return "Пропуск";
    case 6: return "Тип события";
    case 7: return "Действие";
    default: return QVariant();
    }
}

void OSearchModel::updateTable(QList<OEvent> M_data)
{
    beginResetModel();
    m_data.clear();
    endResetModel();

    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data = M_data;
    endInsertRows();
}
