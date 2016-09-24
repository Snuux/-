#include "controller.h"

Controller::Controller(int _id, int _numController, int _typeController, Als *parent) : QObject(0)
{
	numController = _numController;
	typeController = _typeController;
	id = _id;

    firstCommand = 1;
}

Controller::Controller()
{
	numController = 0;
	typeController = 0;
    firstCommand = 1;
}

//Конструктор копирования
Controller::Controller(const Controller &other)
{
	id = other.id;
	numController = other.numController;
	typeController = other.typeController;
	events = other.events;
	brotherController = other.brotherController;
    firstCommand = other.firstCommand;

	parentAls = other.parentAls; //устанавливаем родительский алс
}

int Controller::getId() const
{
	return id;
}

void Controller::setTypeController(int value)
{
	typeController = value;
}

int Controller::getNumController() const
{
	return numController;
}

int Controller::getTypeController() const
{
	return typeController;
}

Controller* Controller::getBrotherController() const
{
	return brotherController;
}

void Controller::setBrotherController(Controller *value)
{
	brotherController = value;
}

void Controller::ansReadTime(QByteArray answer)
{
	//int hour = answer[0];
	//int minute = answer[1];
	//int day = answer[2];
	//int month = answer[3];

	//Считываем время с контроллера (не используется)
	//qDebug() << quint16(hour) << quint16(minute) << quint16(day) << quint16(month);
}

void Controller::setNumController(int value)
{
    numController = value;
}

QString Controller::toBinString(unsigned char state)
{
    QString ret = QString::number(state, 2);

    int nullsCount = 8 - ret.count();

    for (int i = 0; i < nullsCount; i++)
        ret.prepend("0");

    return ret;
}

bool Controller::getBitCount(unsigned char b, int position)
{
    return (b >> position) & 0x1;
}

Als *Controller::getParentAls() const
{
	return parentAls;
}

void Controller::setParentAls(Als *value)
{
	parentAls = value;
}

void Controller::ansReadState(QByteArray answer)
{
    controllerState = answer[0]; //слово-состояние

    if (typeController == 1)
    {
        unsigned char state = answer[1]; //снято/поставленно

        if (getBitCount(controllerState, 4)) //буфер событий не пуст. (4-ый бит == 1)
        {
            qDebug() << "buffer of events not empty type:" << typeController;
            //отправляем чтение из памяти
            parentAls->sendCommandToServer(comReadMemory());
        }
        else
        {
            //читаем посл событие
            ControllerEvent currentEvent(QDate::currentDate(), QTime::currentTime(), state, 0);
            events.push_back(currentEvent);

            endRecieveData();
        }
    }
    else
    {
        //Читаем посл событие
        parentAls->sendCommandToServer(comReadLastEvent());
    }
}

void Controller::ansReadLastEvent(QByteArray answer)
{
    QByteArray key; //ключ
    for (int j = 0; j < 6; j++) key[j] = answer[j];

    if (getBitCount(controllerState, 4)) //буфер событий не пуст. (4-ый бит == 1)
    {
        qDebug() << "buffer of events not empty type:" << typeController;

        parentAls->sendCommandToServer(comReadMemory());
    }
    else
    {
        ControllerEvent currentEvent(QDate::currentDate(), QTime::currentTime(), answer[6], 0, key);
        events.push_back(currentEvent);
        //qDebug() << "answer[6]" << QString::number(answer[6], 2);
        //qDebug() << "   a events[0].state" << events[0].state;
        endRecieveData();
    }
}

void Controller::proceedEvents()
{
    //сортируем по времени, дате
    //выводим на экран

    if (firstCommand == 1)
    {
        if (typeController == 1)//hack
            previousEvent.state = window->getLastStateFromController(id)[0];
        else
            previousEvent.state = window->getLastStateFromController(id)[1];

        firstCommand = 0;
    }

    if (events.count() == 1)
    {
        if (previousEvent.state != events.last().state)
        {
            events.last().analyzeEvent(previousEvent.state, events.last().state, typeController);
            previousEvent.debug();
            events.last().debug();
            QString str1 = "Считано событие: ";
            str1 += events.last().toString();

            window->toConsole(str1);

            previousEvent = events.last();
        }
        else
        {
            events.removeAt(0);
        }

        return;
    }

    qStableSort(events.begin(), events.end(), controllerEventTimeDateLessThan);

    QList<ControllerEvent>::iterator curr = events.begin() + 1;
    previousEvent = *(curr - 1);

    while (curr != events.end())
    {
        if (previousEvent.state != curr->state)
        {
            curr->analyzeEvent(previousEvent.state, curr->state, typeController);

            QString str1 = "Считано событие: ";
            str1 += curr->toString();

            window->toConsole(str1);
        }
        else
        {
            curr = events.erase(curr); //удаляем эл-т из массива
        }

        previousEvent = *(curr - 1); //обновляем пред.
        ++curr;
    }
}

void Controller::clearAfterRead()
{
    parentAls->sendCommandToServer(comClearMemory()); //стираем события из памяти контроллера
    events.clear(); //стираем события из массива
}

void Controller::checkAllarm()
{
	//объединяем массивы
	//сортируем по времени, дате
	//генерируем тревогу

	QList<ControllerEvent> e;
	e.append(events);
	e.append(brotherController->events);

	qStableSort(e.begin(), e.end(), controllerEventTimeDateLessThan);

	for (int i = 0; i < e.count(); i++)
	{
		int current = i;

		if (e[i].event == 1) //если событие "открытие двери"
		{
			for (int j = i; j >= 0; j--)
			{
				if (e[current].klemma == e[j].klemma) //смотрим на предыдущие событие с одиноковой клемой
				{
					if (e[j].event == 3) //если предыдущее событие "Поставлено на охрану"
					{
						//тревога
                        DB::insertAllarm(e[j], id);
						qDebug() << "!!!ALLARM!!!";
						break;
					}
					else if (e[j].event == 4) //если предыдущее событие "Снято с охраны"
						break;
				}

			}
		}
    }
}

void Controller::updateDB()
{
    QList<ControllerEvent> e;
    e.append(events);
    e.append(brotherController->events);

    qStableSort(e.begin(), e.end(), controllerEventTimeDateLessThan);

    for (int i = 0; i < e.count(); i++)
        DB::insertEvent(e[i], id);

    DB::updateControllerState(numController, typeController, events.last().state);
    window->updateControllersTable(parentAls->getId());
}

bool Controller::controllerEventTimeDateLessThan(const ControllerEvent &s1, const ControllerEvent &s2)
{
    if (s1.date < s2.date)
	{
        return s1.time < s2.time;
	}
	else
	{
        return s1.time < s2.time;
	}
}

void Controller::ansReadMemory(QByteArray answer, int bytesCount)
{
    //в зависимости от типа контроллера читаем ответ по разному
    if (typeController == 1)
	{
        int dataBytes = 6; //
        for (int i = 0; i < bytesCount - 1; i += dataBytes)
        {
            qDebug() << "1i" << i << bytesCount;

            QTime time;
            time.setHMS(answer[i + 1], answer[i + 2], QTime::currentTime().second());
            QDate date;
            if (!date.setDate(QDate::currentDate().year(), answer[i + 4], answer[i + 3]))
                date = QDate::currentDate();

            ControllerEvent currentEvent(date, time, answer[i], 1); //текущее событие
            events.push_back(currentEvent);
		}
	}
	else
	{
        int dataBytes = 11; //6
        for (int i = 0; i < bytesCount - 1; i += dataBytes)
		{
            qDebug() << "2i" << i << bytesCount;

            QTime time;
            time.setHMS(answer[i + 1], answer[i + 2], QTime::currentTime().second());
            QDate date;
            if (!date.setDate(QDate::currentDate().year(), answer[i + 4], answer[i + 3]))
                date = QDate::currentDate();
            QByteArray key;
            for (int j = 0; j < 6; j++) key[j] = answer[i + j];

            ControllerEvent currentEvent(date, time, answer[i+6], 1, key); //текущее событие
            events.push_back(currentEvent);
		}
    }

    endRecieveData();
}

void Controller::endRecieveData()
{
    //window->toConsole(QString("[Считывание контроллера ").append(QString::number(numController)).append(" завершено]")
    //    .append(" [тип ").append(QString::number(typeController)).append("]\r"));

    proceedEvents();

    //emit ansComplete(); //если буфер событий пуст, то говорим, что закончили чтение

    updateDB();
    checkAllarm(); //проверяем тревогу

    //clearAfterRead(); //Стираем массив и элементы из памяти контроллера
}

void Controller::slotReadMemory(int _numController, int _typeController, QByteArray answer, int bytesCount)
{
	if ((numController == _numController) && (typeController == _typeController))
	{
		switch (currentCommandCode) {
		case 129: { //81h Прочитать слово-состояние контроллера
			ansReadState(answer);

			break;
		}
		case 130: { //82h Чтение текущего времени контроллера
			ansReadTime(answer);

			break;
		}
        case 131: { //83Н Чтение памяти событий контроллера,
			ansReadMemory(answer, bytesCount);

			break;
		}
        case 132: { //84Н Чтение последнего события,
            ansReadLastEvent(answer);

            break;
        }
		default:
			break;
		}
	}
}

unsigned char Controller::crc(QByteArray command, int bytesCount)
{
	unsigned char c = 0;
	for (int i = 0; i <= bytesCount; i++)
		c = c^command[i];

	return c;
}

QByteArray Controller::comReadLastEvent()
{
    QByteArray command;
    command[0] = 13;
    command[1] = 10;
    command[2] = numController;  //N контр.
    command[3] = typeController; //Тип контр.
    command[4] = 1;

    QString com = "84";
    bool ok;
    command[5] = com.toInt(&ok, 16);
    currentCommandCode = command[5];

    unsigned char i = crc(command, 5);
    command[6] = i;

    return command;
}

QByteArray Controller::comReadState()
{
	QByteArray command;
	command[0] = 13;
	command[1] = 10;
	command[2] = numController;  //N контр.
	command[3] = typeController; //Тип контр.
	command[4] = 1;

	QString com = "81";
	bool ok;
	command[5] = com.toInt(&ok, 16);
	currentCommandCode = command[5];

	unsigned char i = crc(command, 5);
	command[6] = i;

	return command;
}

QByteArray Controller::comWriteKey(QString key)
{
    QByteArray command;
    command[0] = 13;
    command[1] = 10;
    command[2] = numController;  //N контр.
    command[3] = typeController; //Тип контр.
    command[4] = 7;

    QString com = "43";
    bool ok;
    command[5] = com.toInt(&ok, 16);
    currentCommandCode = command[5];

    QByteArray bKey = QByteArray::fromHex(key.toLocal8Bit());

    for (int i = 0; i < 6; i++)
        command[i+6] = bKey[i];

    unsigned char i = crc(command, 11);
    command[12] = i;

    QString str = "\rКлюч ";
    str += key + " установлен";
    window->toConsole(str);

    return command;
}

QByteArray Controller::comWriteTime()
{
	QByteArray command;
	command[0] = 13;
	command[1] = 10;
	command[2] = numController;  //N контр.
	command[3] = typeController; //Тип контр.
	command[4] = 6;

	QString com = "41";
	bool ok;
	command[5] = com.toInt(&ok, 16);
	currentCommandCode = command[5];

	QDate dateToday = QDate::currentDate();
	QTime timenow = QTime::currentTime();
	command[6] = timenow.hour();
	command[7] = timenow.minute();
	command[8] = dateToday.day();
	command[9] = dateToday.month();
	command[10] = 2000 - dateToday.year();

	unsigned char i = crc(command, 10);
	command[11] = i;

    QString str = "Успешно установлено текущее время и дата на контроллере ";
	str += QString::number(numController).append(" [тип ");
    str += QString::number(typeController).append("]");
	window->toConsole(str);

	return command;
}

QByteArray Controller::comReadTime()
{
	QByteArray command;
    command[0] = 13;                //0D
    command[1] = 10;                //0A
    command[2] = numController;     //N контр.
    command[3] = typeController;    //Тип контр.
    command[4] = 1;                 //кол-во байт

    QString com = "82";             //номер команды 82H
    bool ok;                        //временная переменная
	command[5] = com.toInt(&ok, 16);//преобразуем 82 в 16 сист.
    currentCommandCode = command[5];//задаем текущий номер команды

	unsigned char i = crc(command, 5);//создаем контр. сумму
	command[6] = i;                //вписываем контр. сумму в конец команды

	return command;
}

QByteArray Controller::comReadMemory()
{
	QByteArray command;
	command[0] = 13;
	command[1] = 10;
	command[2] = numController;  //N контр.
	command[3] = typeController; //Тип контр.
	command[4] = 1;

	QString com = "83";
	bool ok;
	command[5] = com.toInt(&ok, 16);
	currentCommandCode = command[5];

	unsigned char i = crc(command, 5);
	command[6] = i;

	return command;
}

QByteArray Controller::comClearMemory()
{
	QByteArray command;
	command[0] = 13;
	command[1] = 10;
	command[2] = numController;  //N контр.
	command[3] = typeController; //Тип контр.
	command[4] = 1;

	QString com = "C3";
	bool ok;
	command[5] = com.toInt(&ok, 16);
	currentCommandCode = command[5];

	unsigned char i = crc(command, 5);
	command[6] = i;

	return command;

}
