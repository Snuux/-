#ifndef CONTROLLERMODEL_H
#define CONTROLLERMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "logic/controller.h"

class ControllerModel : public QAbstractTableModel
{
	Q_OBJECT
private:
    QList<QList<QString> > m_data;

public:
	ControllerModel();

public:
	int rowCount(const QModelIndex &parent) const;
	int rowCount() const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	public slots:
	void updateTable(QList<QList<QString> > _m_data);
};

#endif // CONTROLLERMODEL_H
