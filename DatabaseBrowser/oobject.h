#ifndef OOBJECT_H
#define OOBJECT_H

#include <QString>
#include <QDebug>

class Oobject
{

private:
    int id;
    QString name;
    QString place; //city+street+house+floor
    QString state1;
    QString state2;

public:
    Oobject(int Id, QString Name, QString Place, int Klemma, int State1, int State2);
    Oobject(QString Name, QString Place);
    Oobject(const Oobject &obj);
    Oobject();

    int getId() const;
    void setId(int value);
    QString getName() const;
    void setName(const QString &value);
    QString getPlace() const;
    void setPlace(const QString &value);
    QString getState1() const;
    void setState1(const int &klemma, int State);
    QString getState2() const;
    void setState2(const int &klemma, int State);

    void print();
};

#endif // OOBJECT_H
