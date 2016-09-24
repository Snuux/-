#include "controllermodel.h"

ControllerModel::ControllerModel()
{

}

int ControllerModel::rowCount(const QModelIndex &parent) const
{
	return m_data.count();
}

int ControllerModel::rowCount() const
{
	return m_data.count();
}

int ControllerModel::columnCount(const QModelIndex &parent) const
{
	return 4;
}

QVariant ControllerModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
	const QList<QString>& obj = m_data[index.row()];

	switch (index.column()) {
	case 0: return obj[0];
	case 1: return obj[1];
    case 2: return Controller::toBinString(obj[2].toInt());
    case 3: return Controller::toBinString(obj[3].toInt());
	default: return QVariant();
	}
}

QVariant ControllerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation != Qt::Horizontal) return QVariant();
	if (role != Qt::DisplayRole) return QVariant();
	switch (section) {
	case 0: return "ID";
	case 1: return "Номер";
	case 2: return "Состояние 1 (Дверь открыта/закрыта)";
	case 3: return "Состояние 2 (Дверь поставлена/снята с охраны";
	default: return QVariant();
	}
}

void ControllerModel::updateTable(QList<QList<QString> > M_data)
{
	beginResetModel();
	m_data.clear();
	endResetModel();

	beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
	m_data = M_data;
	endInsertRows();
}
