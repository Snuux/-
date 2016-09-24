#ifndef OOBJECTMODEL_H
#define OOBJECTMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QList>

#include "oobject.h"

class OobjectModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<Oobject> m_data;

public:
    OobjectModel();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void updateTable(QList<Oobject> M_data);

};

#endif // OOBJECTMODEL_H
