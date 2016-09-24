#ifndef OALARMMODEL_H
#define OALARMMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QList>
#include <QMessageBox>
#include <QTime>

#include <QAbstractButton>
#include <QPushButton>

#include "newalarmdialog.h"

#include "oevent.h"
#include "bdqueries.h"

class OAlarmModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QList<OEvent> m_data;

public:
    QList<NewAlarmDialog*> dialogs;
    OAlarmModel();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void updateTable(QList<OEvent> M_data, bool isGlobal); //isGlobal - если нужно полностью обновить, а не добавить строки
};

#endif // OALARMMODEL_H
