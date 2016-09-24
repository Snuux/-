#include "oevent.h"

QDateTime OEvent::getDt() const
{
    return dt;
}

void OEvent::setDt(const QDateTime &value)
{
    dt = value;
}

int OEvent::getAct_num() const
{
    return act_num;
}

void OEvent::setAct_num(int value)
{
    act_num = value;
}

QTime OEvent::getReactTime() const
{
    return reactTime;
}

void OEvent::setReactTime(const QTime &value)
{
    reactTime = value;
}

int OEvent::getType() const
{
    return type;
}

void OEvent::setType(int value)
{
    type = value;
}

int OEvent::getNum_key() const
{
    return num_key;
}

void OEvent::setNum_key(int value)
{
    num_key = value;
}

OEvent::OEvent(int Id, int Type, Oobject Obj, int Act, bool Skip, int Num_key, QDate Date, QTime Time, QDateTime Dt)
{
    id = Id;
    type = Type;
    obj = Obj;
    setAct(Act);
    act_num = Act;
    skip = Skip;
    date = Date;
    time = Time;
    dt = Dt;
    num_key = Num_key;
}

OEvent::OEvent(int Id, int Type, Oobject Obj, bool Skip, QDate Date, QTime Time, QDateTime Dt, QTime ReactTime)
{
    id = Id;
    type = Type;
    obj = Obj;
    skip = Skip;
    date = Date;
    time = Time;
    dt = Dt;
    reactTime = ReactTime;
}

OEvent::OEvent(int Id, int Type, Oobject Obj, bool Skip, QDate Date, QTime Time)
{
    id = Id;
    type = Type;
    obj = Obj;
    skip = Skip;
    date = Date;
    time = Time;
}

int OEvent::getId() const
{
    return id;
}

void OEvent::setId(int value)
{
    id = value;
}

Oobject OEvent::getObj() const
{
    return obj;
}

void OEvent::setObj(const Oobject &value)
{
    obj = value;
}

QString OEvent::getAct() const
{
    return act;
}

void OEvent::setAct(const int &value)
{
    switch(value){
    case 1: act = "Дверь закрыта"; break;
    case 2: act = "Дверь открыта"; break;
    case 3: act = "Снято с охраны"; break;
    case 4: act = "Установлено на охрану"; break;
    default: act = ""; break;
    }
}

bool OEvent::getSkip() const
{
    return skip;
}

void OEvent::setSkip(bool value)
{
    skip = value;
}

QDate OEvent::getDate() const
{
    return date;
}

void OEvent::setDate(const QDate &value)
{
    date = value;
}

QTime OEvent::getTime() const
{
    return time;
}

void OEvent::setTime(const QTime &value)
{
    time = value;
}
