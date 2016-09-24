#include "db.h"

QString DB::name;
QSqlDatabase DB::db;

DB::DB()
{

}

QSqlError DB::openDB(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port)
{
	QSqlError err;

	name = dbName;
	db = QSqlDatabase::addDatabase(driver, QString(dbName));
	db.setDatabaseName(dbName);
	db.setHostName(host);
	db.setPort(port);

	if (!db.open(user, passwd)) {
		err = db.lastError();
		db = QSqlDatabase();
		QSqlDatabase::removeDatabase(QString(dbName));
	}

	return err;
}

QList<Controller> DB::getControllersForAls(int alsId)
{
	QList<Controller> ret;

	if (db.isOpen())
	{
		QSqlQuery query(db);
		query.prepare("select id_contr, num_contr from ohrana.controls where id_als = ?;");
		query.bindValue(0, alsId);
		query.exec();

		qDebug() << query.lastError();

		while (query.next()) {
			int id = query.value(0).toInt();
			int numController = query.value(1).toInt();

			Controller c(id, numController, 1, 0); //создаем контроллер с  типом = 1
			ret.push_back(c);
		}
	}

	return ret;
}

QList<QList<QString> > DB::getFullControllersForAls(int alsId)
{
	QList<QList<QString> > ret;

	if (db.isOpen())
	{
		QSqlQuery query(db);
		query.prepare("select id_contr, num_contr, state1, state2 from ohrana.controls where id_als = ?;");
		query.bindValue(0, alsId);
		query.exec();

		qDebug() << query.lastError();

		while (query.next()) {
			QString id = query.value(0).toString();
			QString numController = query.value(1).toString();
			QString state1 = query.value(2).toString();
			QString state2 = query.value(3).toString();

			QList<QString> a;
			a.push_back(id);
			a.push_back(numController);
			a.push_back(state1);
			a.push_back(state2);

			ret.push_back(a);
		}
	}

	return ret;
}

QList<QList<QString> > DB::getAllAls()
{
	QList<QList<QString> > ret;

	if (db.isOpen())
	{
		QSqlQuery query(db);
		query.exec("select id_als, ip, port from ohrana.als");

		qDebug() << query.lastError();

		while (query.next()) {
			QString id = query.value(0).toString();
			QString ip = query.value(1).toString();
			QString port = query.value(2).toString();

			QList<QString> a;
			a.push_back(id);
			a.push_back(ip);
			a.push_back(port);

			ret.push_back(a);
		}
	}

    return ret;
}

ControllerEvent DB::getLastEvent()
{
    ControllerEvent ev;

    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.exec("select e.date_ev, e.time_ev, e.type_event, e.num_key, e.propusk, c.state1, c.state2 from ohrana.events e, ohrana.controls c, ohrana.objects o "
                   "where e.id_obj = o.id_obj and o.id_control = c.id_contr and "
                   "id_event = currval('ohrana.events_id_event_seq'); ");

        while (query.next())
        {
            ev.date = query.value(0).toDate();
            ev.time = query.value(1).toTime();
            ev.event = query.value(2).toInt();
            ev.key = query.value(3).toByteArray();
            ev.propusk = query.value(4).toInt();

            if (ev.event == 1 || ev.event == 2)
                ev.state = (unsigned char) query.value(5).toInt();
            else
                ev.state = (unsigned char) query.value(6).toInt();
        }
    }

    return ev;
}

unsigned char DB::getControllerState1(int controllerId, int controllerType)
{
    unsigned char state;

    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.prepare("select c.state1, c.state2 from ohrana.controls c "
                   "where c.id_contr = ?; ");
        query.bindValue(0, controllerId);
        query.exec();

        while (query.next())
        {
            if (controllerType == 1 || controllerType == 2)
                state = query.value(0).toInt();
            else
                state = query.value(6).toInt();
        }
    }

    return state;
}

//unsigned char DB::getControllerState2(int controllerId)
//{
//
//}

void DB::insertEvent(ControllerEvent &ev, int controllerId)
{
	if (db.isOpen())
	{
        int objectId = -1;

        qDebug() << "controllerId" << controllerId << "ev.klemma" << ev.klemma;

        QSqlQuery query1(db);
        query1.prepare("select id_obj from ohrana.objects where id_control = ? and num_klemma = ?");
        query1.bindValue(0, controllerId);
        query1.bindValue(1, ev.klemma);
        query1.exec();

        while (query1.next()) {
            objectId = query1.value(0).toInt();
        }

		QSqlQuery query(db);
		query.prepare("INSERT INTO ohrana.events (id_event, id_obj, date_ev, time_ev, type_event, num_key, insert_time, propusk) "
			"VALUES (nextval('ohrana.events_id_event_seq'), ?, ?, ?, ?, ?, ?, ?)");

        query.bindValue(0, objectId);
		query.bindValue(1, ev.date);
		query.bindValue(2, ev.time);
		query.bindValue(3, ev.event);
		query.bindValue(4, ev.key.toHex().toInt(0, 16));
		query.bindValue(5, QDateTime::currentDateTime());
		query.bindValue(6, ev.propusk);

		query.exec();

		qDebug() << query.lastError();
	}
}

void DB::updateControllerState(int controllerNum, int controllerType, unsigned char state)
{
    //qDebug() << "controllerNum" << controllerNum << "controllerType" << controllerType
    //         << "state" << state;

    if (db.isOpen())
	{
		if (controllerType == 1)
		{
			QSqlQuery query(db);
            query.prepare("UPDATE ohrana.controls SET state1 = ? WHERE num_contr = ?");
            query.bindValue(0, state);
			query.bindValue(1, controllerNum);

			query.exec();

            //qDebug() << query.lastError();
		}
		else
		{
			QSqlQuery query(db);
            query.prepare("UPDATE ohrana.controls SET state2 = ? WHERE num_contr = ?");
            query.bindValue(0, state);
			query.bindValue(1, controllerNum);

			query.exec();

            //qDebug() << query.lastError();
		}
    }
}

void DB::insertAllarm(int objectID, QDate date, QTime time, QDateTime insertTime, int propusk)
{
	if (db.isOpen())
	{
		QSqlQuery query(db);
		query.prepare("INSERT INTO ohrana.allarm (id_allarm, id_obj, date_ev, time_ev, insert_time, propusk, reaction_time) "
			"VALUES (nextval('ohrana.allarm_id_allarm_seq'), ?, ?, ?, ?, ?, ?)");

		query.bindValue(0, objectID);
		query.bindValue(1, date);
		query.bindValue(2, time);
		query.bindValue(3, insertTime);
		query.bindValue(4, propusk);
		query.bindValue(5, QDateTime::currentDateTime());

		query.exec();
	}
}

void DB::insertAllarm(ControllerEvent &ev, int controllerId)
{
	if (db.isOpen())
	{
		int objectId = -1;

		QSqlQuery query1(db);
		query1.prepare("select id_obj from ohrana.objects where id_control = ? and num_klemma = ?");
		query1.bindValue(0, controllerId);
		query1.bindValue(1, ev.klemma);
		query1.exec();

		while (query1.next()) {
			objectId = query1.value(0).toInt();
		}

		QSqlQuery query2(db);
		query2.prepare("INSERT INTO ohrana.allarm (id_allarm, id_obj, date_ev, time_ev, insert_time, propusk, reaction_time) "
			"VALUES (nextval('ohrana.allarm_id_allarm_seq'), ?, ?, ?, ?, ?, ?)");

		query2.bindValue(0, objectId);
		query2.bindValue(1, ev.date);
		query2.bindValue(2, ev.time);
		query2.bindValue(3, QDateTime::currentDateTime());
		query2.bindValue(4, ev.propusk);
		query2.bindValue(5, QVariant(QVariant::Time));

		query2.exec();
	}
}


