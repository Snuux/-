#include "oobject.h"

Oobject::Oobject(int Id, QString Name, QString Place, int Klemma, int State1, int State2)
{
    id = Id;
    name = Name;
    place = Place;
    setState1(Klemma, State1);
    setState2(Klemma, State2);
}

Oobject::Oobject(QString Name, QString Place)
{
    id = 0;
    name = Name;
    place = Place;
    state1 = "";
    state2 = "";
}

Oobject::Oobject(const Oobject &obj)
{
    id = obj.id;
    name = obj.name;
    place = obj.place;
    state1 = obj.state1;
    state2 = obj.state2;
}

Oobject::Oobject()
{
    id = 0;
    name = "";
    place = "";
    state1 = "";
    state2 = "";
}

int Oobject::getId() const
{
    return id;
}

void Oobject::setId(int value)
{
    id = value;
}


QString Oobject::getState2() const
{
    return state2;
}

void Oobject::setState2(const int &klemma, int State)
{
    int x = 0;
    for (int i = 7; i >= 0; i--)
    {
        x = ((State >> i) & 1); //Из десятичной в двоичную
        if (8 - i == klemma)
            break;
    }

    switch(x)
    {
    case 0: state2 = "Снято с охраны"; break;
    case 1: state2 = "Установлено на охрану"; break;
    default: state2 = "Снято с охраны";//"не известно";
    }
}

QString Oobject::getState1() const
{
    return state1;
}

void Oobject::setState1(const int &klemma, int State)
{
    int x = 0;
    for (int i = 7; i >= 0; i--)
    {
        x = ((State >> i) & 1); //Из десятичной в двоичную
        if (8 - i == klemma)
            break;
    }

    switch(x)
    {
    case 0: state1 = "Закрыто"; break;
    case 1: state1 = "Открыто"; break;
    default: state1 = "Закрыто";//"не известно";
    }
}

QString Oobject::getPlace() const
{
    return place;
}

void Oobject::setPlace(const QString &value)
{
    place = value;
}

QString Oobject::getName() const
{
    return name;
}

void Oobject::setName(const QString &value)
{
    name = value;
}

void Oobject::print()
{
    qDebug() << name << place << state1 << state2;
}
