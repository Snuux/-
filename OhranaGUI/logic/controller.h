#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtNetwork>
#include <QObject>
#include <algorithm>

#include "als.h"
#include "db.h"
#include "controllerevent.h"
#include "main.h"

class Als;

class Controller : public QObject
{
	Q_OBJECT

private:
    int id; //ID контроллера
    int numController; //номер контроллера
    int typeController; //тип контроллера

    unsigned char controllerState; //слово-состояние контроллера
    unsigned char currentCommandCode; //текущая команда

    Controller* brotherController; //контроллер-брат

	QList<ControllerEvent> events; //все события
	ControllerEvent previousEvent; //пред. событие

	bool getBitCount(unsigned char b, int position); //получаем опред. бит из числа
	Als *parentAls; //родительский Алс

    int firstCommand; //если выполняется первая команда - то 1, иначе 0

public:
	explicit Controller(int _id, int _numController, int _typeController, Als *parent = 0);
	explicit Controller();
	Controller(const Controller &other);

	unsigned char crc(QByteArray command, int bytesCount);

	QByteArray comReadState(); //команды контроллера
	QByteArray comWriteTime();
	QByteArray comReadTime();
	QByteArray comReadMemory();
	QByteArray comClearMemory();
    QByteArray comWriteKey(QString key);
    QByteArray comReadLastEvent();

	void ansReadState(QByteArray answer); //ответ на команды контроллера
	void ansWriteTime(QByteArray answer);
	void ansReadTime(QByteArray answer);
	void ansReadMemory(QByteArray answer, int bytesCount);
	void ansClearMemory(QByteArray answer);
    void ansReadLastEvent(QByteArray answer);

    void endRecieveData(); //выполняется после заершения чтения

	static bool controllerEventTimeDateLessThan(const ControllerEvent &s1, const ControllerEvent &s2);

    void setBrotherController(Controller *value); //устанавливаем контроллер брат
    void setTypeController(int value); //устанавливаем тип контроллера

    void checkAllarm(); //проверяем тревоги
    void updateDB(); //обновляем БД
    void proceedEvents(); //обрабатываем и выводим события в консоль
    void clearAfterRead(); //удаляем события из памяти после получения и обработки

    Controller* getBrotherController() const;
	int getId() const;
	int getNumController() const;
	int getTypeController() const;
	Als *getParentAls() const; //подключаем ansComplete
	void setParentAls(Als *value);

    void setNumController(int value);

    static QString toBinString(unsigned char state); //проебразуем строку в двоичный формат

signals:
    void ansComplete(); //вызываем после чтения памяти

	public slots:
	void slotReadMemory(int _numController, int _typeController, QByteArray answer, int bytesCount);
};

#endif // CONTROLLER_H
