#ifndef BDQUERIES_H
#define BDQUERIES_H

#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QList>
#include <QSqlRecord>
#include <QSqlError>

#include <QDate>
#include <QTime>
#include <QDateTime>

#include "oobject.h"
#include "oevent.h"

class BdQueries
{
public:
    BdQueries();

    static QList<Oobject> getObjects();
    static QList<OEvent> getEvents(QDate currentDate);
    static QList<OEvent> getOhrans(QDate currentDate);
    static QList<OEvent> getAlarms(QDate currentDate);

    static QList<OEvent> getAll(QString city, QString street, QString house, QTime fromT, QTime toT, QDate fromD, QDate toD, QString objName, int type);
    static const QString dbName;

    static void setAlarmReactionTime(int id, QTime t);
    static void insertAllarm();
};



#endif // BDQUERIES_H
