#ifndef OOHRANAMODEL_H
#define OOHRANAMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QList>

#include "oevent.h"

class OOhranaModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<OEvent> m_data;

public:
    OOhranaModel();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void updateTable(QList<OEvent> M_data);
};

#endif // OOHRANAMODEL_H
