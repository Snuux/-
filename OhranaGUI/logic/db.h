#ifndef BD_H
#define BD_H

#include <QList>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>

#include "als.h"
#include "controller.h"

class Controller;
class ControllerEvent;

class DB
{
public:
	DB();

	static QString name;
	static QSqlDatabase db;

	static QSqlError openDB(const QString &driver, const QString &dbName, const QString &host,
		const QString &user, const QString &passwd, int port = -1);

	//получение контроллеров к алс (вызывается в классе Als)
	static QList<Controller> getControllersForAls(int alsId);

	//получение контроллеров к алс (используется для отображения в таблице)
	static QList<QList<QString> > getFullControllersForAls(int alsId);

	//TODO получение всех алс (используется для опроса событий)
	static QList<QList<QString> > getAllAls();

	static QList<int> getObjectsIDs();

    static ControllerEvent getLastEvent();

    static unsigned char getControllerState1(int controllerId, int controllerType);

    static void insertEvent(ControllerEvent& ev, int numController);
	static void updateControllerState(int controllerNum, int controllerType, unsigned char state);
	static void insertAllarm(int objectID, QDate date, QTime time, QDateTime insertTime, int propusk);
    static void insertAllarm(ControllerEvent& ev, int controllerId);
};

#endif // BD_H
