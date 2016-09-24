#include "oobjectmodel.h"

OobjectModel::OobjectModel()
{

}

int OobjectModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

int OobjectModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant OobjectModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
    const Oobject& obj = m_data[index.row()];

    switch(index.column()){
    case 0: return obj.getId();
    case 1: return obj.getName();
    case 2: return obj.getPlace();
    case 3: return obj.getState1();
    case 4: return obj.getState2();
    default: return QVariant();
    }
}

QVariant OobjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case 0: return "ID";
    case 1: return "Объект";
    case 2: return "Адрес";
    case 3: return "Состояние 1";
    case 4: return "Состояние 2";
    default: return QVariant();
    }
}

void OobjectModel::updateTable(QList<Oobject> M_data)
{
    beginResetModel();
    m_data.clear();
    endResetModel();

    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data = M_data;
    endInsertRows();


}
