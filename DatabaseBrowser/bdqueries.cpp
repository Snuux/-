#include "bdqueries.h"

#include <QSqlDatabase>

const QString BdQueries::dbName = "Ohrana";

BdQueries::BdQueries()
{
}

QList<Oobject>  BdQueries::getObjects()
{
    QList<Oobject> ret;
    qDebug() << "getObjects()";

    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.exec("select o.id_obj, o.name, c.city, s.street, h.num_house, o.num_klemma, co.state1, co.state2 from ohrana.objects o, ohrana.city c, ohrana.street s, ohrana.house h, ohrana.als a, ohrana.controls co "
                   "where o.id_house = h.id_house and h.id_street = s.idstreet and s.idcity = c.idcity and o.id_control = co.id_contr and a.id_als = co.id_als;");
        while (query.next()) {
           int id =          query.value(0).toInt();//query.value(fId).toString().toInt();
           QString name =    query.value(1).toString();//query.value(fName).toString();
           QString place =   "";//query.value(2).toString(); place += ", "; //query.value(fCity).toString();   place += " ";
           place +=          query.value(3).toString(); place += ", "; //query.value(fStreet).toString(); place += " ";
           place +=          query.value(4).toString(); //query.value(fHouse).toString();  place += " ";
           int klemma =      query.value(5).toString().toInt();//query.value(fState1).toString().toInt();
           int state1 =      query.value(6).toString().toInt();//query.value(fState1).toString().toInt();
           int state2 =      query.value(7).toString().toInt();//query.value(fState2).toString().toInt();


           Oobject o(id, name, place, klemma, state1, state2);
           ret.push_back(o);
        }

        //qDebug() << "outObjects()";
        //for (int i = 0; i < ret.count(); i++)
        //    ret[i].print();
    }

    return ret;
}

QList<OEvent> BdQueries::getEvents(QDate currentDate)//TODO
{
    QList<OEvent> ret;
    qDebug() << "getEvents()";

    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.prepare("select e.id_event, o.name, c.city, s.street, h.num_house, e.date_ev, e.time_ev, e.type_event, e.num_key, e.insert_time, e.propusk from ohrana.events e, ohrana.objects o, ohrana.house h, ohrana.city c, ohrana.street s "
                   "where e.id_obj = o.id_obj and o.id_house = h.id_house and h.id_street = s.idstreet and s.idcity = c.idcity and e.type_event < 3 and (e.date_ev = ? or '1752-09-14' = ?)");
        //if (currentDate.day() = 0 && currentDate.month() = 0 && currentDate.year() = 0)
        //{
            //qDebug() << "yessss";
            query.bindValue(0, currentDate);
            query.bindValue(1, currentDate.toString("yyyy-MM-dd"));
        //}
        //else


        query.exec();


        while (query.next()) {
           int id =          query.value(0).toInt();//query.value(fId).toString().toInt();
           QString name =    query.value(1).toString();//query.value(fName).toString();
           QString place =   "";//query.value(2).toString(); place += ", "; //query.value(fCity).toString();   place += " ";
           place +=          query.value(3).toString(); place += ", "; //query.value(fStreet).toString(); place += " ";
           place +=          query.value(4).toString(); //query.value(fHouse).toString();  place += " ";
           QDate date =      query.value(5).toDate();
           QTime time =      query.value(6).toTime();
           int type   =      query.value(7).toInt();
           int num   =       query.value(8).toInt();
           QDateTime dt =    query.value(9).toDateTime();
           int propusk =     query.value(10).toInt();

          OEvent ev(id, 1, Oobject(name, place), type, propusk, num, date, time, dt);
          ret.push_back(ev);
        }
    }

    return ret;
}

QList<OEvent> BdQueries::getOhrans(QDate currentDate)
{
    QList<OEvent> ret;
    qDebug() << "getAlarms()";

    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.prepare("select e.id_event, o.name, c.city, s.street, h.num_house, e.date_ev, e.time_ev, e.type_event, e.num_key, e.insert_time, e.propusk from ohrana.events e, ohrana.objects o, ohrana.house h, ohrana.city c, ohrana.street s "
                   "where e.id_obj = o.id_obj and o.id_house = h.id_house and h.id_street = s.idstreet and s.idcity = c.idcity and e.type_event > 2 and (e.date_ev = ? or '1752-09-14' = ?)");
        query.bindValue(0, currentDate);
        query.bindValue(1, currentDate.toString("yyyy-MM-dd"));
        query.exec();


        while (query.next()) {
           int id =          query.value(0).toInt();//query.value(fId).toString().toInt();
           QString name =    query.value(1).toString();//query.value(fName).toString();
           QString place =   "";//query.value(2).toString(); place += ", "; //query.value(fCity).toString();   place += " ";
           place +=          query.value(3).toString(); place += ", "; //query.value(fStreet).toString(); place += " ";
           place +=          query.value(4).toString(); //query.value(fHouse).toString();  place += " ";
           QDate date =      query.value(5).toDate();
           QTime time =      query.value(6).toTime();
           int type   =      query.value(7).toInt();
           int num   =       query.value(8).toInt();
           QDateTime dt =    query.value(9).toDateTime();
           int propusk =     query.value(10).toInt();

          OEvent ev(id, 2, Oobject(name, place), type, propusk, num, date, time, dt);
          ret.push_back(ev);
        }
    }

    return ret;
}

QList<OEvent> BdQueries::getAlarms(QDate currentDate)
{
    QList<OEvent> ret;
    qDebug() << "getAlarms()";

    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (db.isOpen())
    {
        QSqlQuery query(db);
        query.prepare("select a.id_allarm, o.name, c.city, s.street, h.num_house, a.date_ev, a.time_ev, a.insert_time, a.propusk, a.reaction_time from ohrana.allarm a, ohrana.objects o, ohrana.house h, ohrana.city c, ohrana.street s "
                   "where a.id_obj = o.id_obj and o.id_house = h.id_house and h.id_street = s.idstreet and s.idcity = c.idcity and (a.date_ev = ? or '1752-09-14' = ?)");
        query.bindValue(0, currentDate);
        query.bindValue(1, currentDate.toString("yyyy-MM-dd"));
        query.exec();

        //qDebug() << query.lastError() << currentDate.toString("yyyy-MM-dd");

        while (query.next()) {
           int id =          query.value(0).toInt();//query.value(fId).toString().toInt();
           QString name =    query.value(1).toString();//query.value(fName).toString();
           QString place =   "";//query.value(2).toString(); place += ", "; //query.value(fCity).toString();   place += " ";
           place +=          query.value(3).toString(); place += ", "; //query.value(fStreet).toString(); place += " ";
           place +=          query.value(4).toString(); //query.value(fHouse).toString();  place += " ";
           QDate date =      query.value(5).toDate();
           QTime time =      query.value(6).toTime();
           QDateTime dt =    query.value(7).toDateTime();
           int propusk =     query.value(8).toInt();
           QTime reactTime = query.value(9).toTime();

          OEvent al(id, 3, Oobject(name, place), propusk, date, time, dt, reactTime);
          ret.push_back(al);
        }
    }

    return ret;
}

QList<OEvent> BdQueries::getAll(QString city, QString street, QString house, QTime fromT, QTime toT, QDate fromD, QDate toD, QString objName, int t)
{
    QList<OEvent> ret;
    qDebug() << "getAll()";

    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (db.isOpen())
    {
        QSqlQuery query(db);

        QString q("select id_event, name, city, street, house, date_ev, time_ev, propusk, type, opt from ohrana.getAll(:city, :street, :house, :fromT, :toT, :fromD, :toD, :objName, ");
        q = q + QString::number(t) + ");";

        //qDebug() << q;
        query.prepare(q);

        query.bindValue(":city", city);
        query.bindValue(":street", street);
        query.bindValue(":house", house);

        query.bindValue(":fromT",   fromT);//QVariant(QVariant::Time));//fromT);
        query.bindValue(":toT",     toT);  //QVariant(QVariant::Time));//toT);
        query.bindValue(":fromD",   fromD);//QVariant(QVariant::Date));//fromD);
        query.bindValue(":toD",     toD);  //QVariant(QVariant::Date));//toD);
        query.bindValue(":objName", objName);

        query.exec();

        //query.lastError();

        qDebug() << query.lastError() << fromD.toString();

        while (query.next()) {
           int id =      query.value(0).toInt();
           QString name =    query.value(1).toString();
           QString place =   "";//query.value(2).toString(); place += ", "; //query.value(fCity).toString();   place += " ";
           place +=          query.value(3).toString(); place += ", ";
           place +=          query.value(4).toString();
           QDate date =      query.value(5).toDate();
           QTime time =      query.value(6).toTime();
           int propusk =     query.value(7).toBool();
           int type =        query.value(8).toInt();
           int opt =         query.value(9).toInt();


           //qDebug() << id << name << type;

           OEvent ev(id, type, Oobject(name, place), propusk, date, time);
           ev.setAct_num(opt);
           ev.setAct(opt);
           ret.push_back(ev);
        }
    }

    return ret;
}

void BdQueries::setAlarmReactionTime(int id, QTime t)
{
    qDebug() << "setAlarmReactionTime()";

    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (db.isOpen())
    {
        QSqlQuery query(db);
        QString q;

        query.prepare("update ohrana.allarm set reaction_time = ?"
                      "where id_allarm = ?");

        query.bindValue(0, t);
        query.bindValue(1, id);

        query.exec();

        qDebug() << query.lastError();


    }
}

void BdQueries::insertAllarm()
{
    qDebug() << "insertAlarm()";

    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (db.isOpen())
    {
        QSqlQuery query(db);
        //select setval('ohrana.allarm_id_allarm_seq', (select max(id_allarm) from ohrana.allarm));
        query.prepare("INSERT INTO ohrana.allarm (id_allarm, id_obj, date_ev, time_ev, insert_time, propusk, reaction_time) "
                      "VALUES (nextval('ohrana.allarm_id_allarm_seq'), ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, qrand() % (8-0) +0);
        query.bindValue(1, QDate().currentDate());
        query.bindValue(2, QTime().currentTime());

        QDateTime d;
        d.setDate(QDate().currentDate());
        QTime t;
        t.setHMS(0, qrand() % (8-0) +0, qrand() % (60-0) +0);
        d.setTime(t);
        query.bindValue(3, d);

        query.bindValue(4, qrand() % (2-0) +0);
        query.bindValue(5, QTime());

        query.exec();

        qDebug() << query.lastError();
    }
}

