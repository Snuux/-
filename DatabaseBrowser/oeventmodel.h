#ifndef OEVENTMODEL_H
#define OEVENTMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QList>

#include "oevent.h"

class OEventModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<OEvent> m_data;

public:
    OEventModel();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void updateTable(QList<OEvent> M_data);
};

#endif // OEVENTMODEL_H
