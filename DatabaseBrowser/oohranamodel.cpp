#include "oohranamodel.h"

OOhranaModel::OOhranaModel()
{

}

int OOhranaModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

int OOhranaModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant OOhranaModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
    const OEvent& ev = m_data[index.row()];

    switch(index.column()){
    case 0: return ev.getId();
    case 1: return ev.getObj().getName();
    case 2: return ev.getObj().getPlace();
    case 3: return ev.getAct();
    case 4: return ev.getTime();
    case 5: return ev.getNum_key();
    case 6: return (ev.getSkip() ? "Есть" : "");
    default: return QVariant();
    }
}

QVariant OOhranaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    switch (section) {
    case 0: return "ID";
    case 1: return "Объект";
    case 2: return "Адрес";
    case 3: return "Действие";
    case 4: return "Время";
    case 5: return "Ключ";
    case 6: return "Пропуск";
    default: return QVariant();
    }
}

void OOhranaModel::updateTable(QList<OEvent> M_data)
{
    beginResetModel();
    m_data.clear();
    endResetModel();

    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data = M_data;
    endInsertRows();
}
