#include "controllerevent.h"

ControllerEvent::ControllerEvent()
{
    time = QTime::currentTime();
    date = QDate::currentDate();
    state = 99;
}

ControllerEvent::ControllerEvent(QDate _date, QTime _time, unsigned char _state, int _propusk, QByteArray _key)
{
    time    = _time   ;
    date    = _date   ;
    state   = _state  ;
    key     = _key    ;
    propusk = _propusk;
}

ControllerEvent::ControllerEvent(const ControllerEvent &other)
{
    time    = other.time   ;
    date    = other.date   ;
    state   = other.state  ;
    key     = other.key    ;
    klemma  = other.klemma ;
    event   = other.event  ;
    propusk = other.propusk;
}

void ControllerEvent::analyzeEvent(int previousState, int currentState, int typeController)
{
    int currentEvent = 0;
    unsigned char ev = previousState ^ currentState; //Исключающее ИЛИ, находим измененные биты

    for (int i = 0; i < 8; i++)
    {
        ev = ev >> 1; //сдвиг на бит вправо/читаем побайтово
        klemma = i;

        if (ev == 0)
        {
            if (typeController == 1)
            {
                if (previousState > currentState) //Если изминилось с 0 до 1, то дверь открыта
                    currentEvent = 1;
                else
                    currentEvent = 2;

                break;
            }
            else
            {
                if (previousState > currentState) //Если изминилось с 0 до 1, то поставлено на охрану
                    currentEvent = 3;
                else
                    currentEvent = 4;
            }
            break;
        }
    }

    event = currentEvent;
}

void ControllerEvent::debug()
{
    QString str = "";
    str += date.toString("MM.dd") + " ";
    str += time.toString("HH:mm") + " State: ";
    str += QString::number(state, 2) + " Klemma: ";
    str += QString::number(klemma) + " Event: ";
    str += QString::number(event) + " Key: ";
    str += key.toHex();

    qDebug() << str;
}

QString ControllerEvent::toString()
{
    QString ret = date.toString("dd.MM.yyyy").append(" ")
            + time.toString("HH:mm:ss").append(". Состояние: ")
            + getBinState().append(" Клемма: ")
            + QString::number(klemma).append(". Событие: ");

    switch(event)
    {
    case 1: ret += "дверь закрыта. ";
        break;
    case 2: ret += "дверь открыта. ";
        break;
    case 3: ret += QString("снято с охраны.  Ключ: ")
                + key.toHex();
        break;
    case 4: ret += QString("поставлено на охрану.  Ключ: ")
                + key.toHex();
        break;
    }

    return ret;
}

QString ControllerEvent::getBinState()
{
    QString ret = QString::number(state, 2);

    //qDebug() << "state" << ret;
    int nullsCount = 8 - ret.count();

    for (int i = 0; i < nullsCount; i++)
        ret.prepend("0");

    return ret;
}
