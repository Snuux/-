#ifndef OEVENT_H
#define OEVENT_H

#include <QString>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QDateTime>

#include "oobject.h"

class OEvent
{
private:
    int id;
    Oobject obj;
    int act_num; //номер события
    QString act; //название события
    bool skip;
    int num_key;
    QDate date;
    QTime time;
    QDateTime dt; //insert_time
    QTime reactTime;
    int type; // двери=1/охрана=2/тревога=3

public:
    OEvent(int Id, int Type, Oobject Obj, int Act, bool Skip, int Num_key, QDate Date, QTime Time, QDateTime Dt);
    OEvent(int Id, int Type, Oobject Obj, bool Skip, QDate Date, QTime Time, QDateTime Dt, QTime ReactTime);
    OEvent(int Id, int Type, Oobject Obj, bool Skip, QDate Date, QTime Time);

    int getId() const;
    void setId(int value);
    Oobject getObj() const;
    void setObj(const Oobject &value);
    QString getAct() const;
    void setAct(const int &value);
    bool getSkip() const;
    void setSkip(bool value);
    QDate getDate() const;
    void setDate(const QDate &value);
    QTime getTime() const;
    void setTime(const QTime &value);
    QDateTime getDt() const;
    void setDt(const QDateTime &value);
    int getAct_num() const;
    void setAct_num(int value);
    QTime getReactTime() const;
    void setReactTime(const QTime &value);
    int getType() const;
    void setType(int value);
    int getNum_key() const;
    void setNum_key(int value);

    OEvent(){}
    /*OEvent(const OEvent& ev)
    {
        id      = ev.id     ;
        type    = ev.type   ;
        obj     = ev.obj    ;
        act     = ev.act    ;
        act_num = ev.act_num;
        skip    = ev.skip   ;
        date    = ev.date   ;
        time    = ev.time   ;
        dt      = ev.dt     ;
        num_key = ev.num_key;
    }
    OEvent& operator=(OEvent& ev)
    {
        id      = ev.id     ;
        type    = ev.type   ;
        obj     = ev.obj    ;
        act     = ev.act    ;
        act_num = ev.act_num;
        skip    = ev.skip   ;
        date    = ev.date   ;
        time    = ev.time   ;
        dt      = ev.dt     ;
        num_key = ev.num_key;

        return *this;
    }

    OEvent& operator=(const OEvent& ev)
    {
        id      = ev.id     ;
        type    = ev.type   ;
        obj     = ev.obj    ;
        act     = ev.act    ;
        act_num = ev.act_num;
        skip    = ev.skip   ;
        date    = ev.date   ;
        time    = ev.time   ;
        dt      = ev.dt     ;
        num_key = ev.num_key;

        return *this;
    }*/
};

#endif // OEVENT_H
