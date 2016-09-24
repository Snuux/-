#ifndef CONTROLLEREVENT_H
#define CONTROLLEREVENT_H

#include <QByteArray>
#include <QDate>
#include <QTime>
#include <QDebug>


class ControllerEvent
{
public:
    QTime time;
    QDate date;
    unsigned char state; //состояние
    QByteArray key; //ключ
    int klemma;
    int event; //номер события 1-4
    int propusk;

    ControllerEvent();
    ControllerEvent(QDate _date, QTime _time, unsigned char _state, int _propusk, QByteArray _key = 0);
    ControllerEvent(const ControllerEvent &other);

    void analyzeEvent(int previousState, int currentState, int typeController);
    void debug();

    QString toString();
    QString getBinState();
};

#endif // CONTROLLEREVENT_H
