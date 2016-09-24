#include "als.h"

Als::Als(int _id, QString _ip, int _port, QObject *parent) : QObject(parent)
{
	id = _id;
	nextBlockSize = 0;

	//создаем новый сокет
	socket = new QTcpSocket(this);

	//подключаемся к als
	socket->connectToHost(_ip, _port, QIODevice::ReadWrite, QAbstractSocket::AnyIPProtocol);
	qDebug() << socket->state();

	//устанавливаем сигналы/слоты к сокету
	connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
	connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(slotError(QAbstractSocket::SocketError)));

	initControllers();

	connect(this, SIGNAL(goodConnect()), SLOT(slotSendToServer()));
}

Als::~Als()
{
	for (int i = 0; i < controllers.count(); i++)
		delete controllers[i];

	controllers.clear();
	delete socket;
}

int Als::getId() const
{
	return id;
}

void Als::initControllers()
{
	//получаем контроллеры из бд
	QList<Controller> r = DB::getControllersForAls(id);

	//для всех контроллеров
	for (int i = 0; i < r.count(); i++)
	{
		Controller *original = new Controller(r[i]); //контроллер с типом 1
		original->setParent(this); //устанавливаем в качестве родителя - этот алс

		Controller *brother = new Controller(r[i]); //задаем связанный контроллер (с типом 2)
		brother->setTypeController(2); //задаем ему 2 тип

		original->setParentAls(this);
		brother->setParentAls(this);

		original->setBrotherController(brother); //устанавливаем контроллер 2 типа в контроллер 1 типа
		brother->setBrotherController(original); //устанавливаем контроллер 1 типа в контроллер 2 типа

		connect(this, SIGNAL(readNewAnswer(int, int, QByteArray, int)), original, SLOT(slotReadMemory(int, int, QByteArray, int)));
		connect(this, SIGNAL(readNewAnswer(int, int, QByteArray, int)), original->getBrotherController(), SLOT(slotReadMemory(int, int, QByteArray, int)));

        controllers.push_back(brother);
        controllers.push_back(original); //добавляем в список контроллеров
	}
}

QTcpSocket *Als::getSocket() const
{
    return socket;
}

void Als::wait(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void Als::slotSendReadStateCommand()
{
	QByteArray  arrBlock;

	for (int i = 0; i < controllers.count(); i++)
	{
        //для всех контроллеров отправляем команду чтения состояния
		arrBlock = controllers[i]->comReadState();
		sendCommandToServer(arrBlock);
	}
}

void Als::slotSendWriteTimeCommand()
{
    QByteArray  arrBlock;

    for (int i = 0; i < controllers.count(); i++)
    {
        arrBlock = controllers[i]->comWriteTime();
        sendCommandToServer(arrBlock);
    }
}

void Als::slotSendClearMemoryCommand()
{
    QByteArray  arrBlock;

    for (int i = 0; i < controllers.count(); i++)
    {
        arrBlock = controllers[i]->comClearMemory();
        sendCommandToServer(arrBlock);
    }
}

void Als::slotSendWriteKeyCommand(int numController, QString key)
{
    QByteArray  arrBlock;

    for (int i = 0; i < controllers.count(); i++)
    {
        if (controllers[i]->getNumController() == numController
            && controllers[i]->getTypeController() == 2)
        {
            arrBlock = controllers[i]->comWriteKey(key);
            sendCommandToServer(arrBlock);
        }
    }
}

void Als::sendCommandToServer(QByteArray com)
{
    //пишем в сокет
    socket->write(com);
	emit send();

	//для ускорения реагирования программы используем processEvents
	QTime time;
	time.start();
	for (; time.elapsed() < 1000;)
	{
		qApp->processEvents();
	}
}

void Als::slotReadyRead()
{
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_5_5);

	qDebug() << "slotReadyRead: " << id;

	while (true) //беск. цикл
	{
		if (nextBlockSize == 0) //если нет уже полученных данных
		{
			//если размер 3-х первых байт < 3 (номер контр., тип контр.,
			//кол-во данных), то выходим
			if (socket->bytesAvailable() < 3) {
				break;
			}

			//получаем 3 первые байта
			char * buf = new char[3];
			in.readRawData(buf, 3);
			QByteArray startBytes(buf, 3);
			delete buf;

            //QString d = "";
            //foreach(unsigned char a, startBytes)
            //	d += QString::number(a, 10).append(" ");
            //qDebug() << "startBytes: " << d;

			currentNumController = startBytes[0];
			currentTypeController = startBytes[1];

			//записываем сколько данных ещё нужно считать
			nextBlockSize = startBytes[2] + 1;
		}

        //qDebug() << "    socket->bytesAvailable(): " << QString::number(socket->bytesAvailable())
        //	<< "    nextBlockSize" << QString::number(nextBlockSize);

		//если есть такое кол-во данных
		if (socket->bytesAvailable() < nextBlockSize) {
			//qDebug() << "    in ";
			break;
		}

		//qDebug() << "    after ";

		//считываем данные в массив
		char *  buf = new char[nextBlockSize];
		in.readRawData(buf, nextBlockSize);
		QByteArray dataBytes(buf, nextBlockSize);
		delete buf;

        //QString d = "";
        //foreach(unsigned char a, dataBytes)
        //	d += QString::number(a, 10).append(" ");
        //qDebug() << "dataBytes: " << d;

        //qDebug() << "currentNumController: " << currentNumController << " currentTypeController: " << currentTypeController;
		emit readNewAnswer(currentNumController, currentTypeController, dataBytes, nextBlockSize);

		nextBlockSize = 0;
	}
}

void Als::slotError(QAbstractSocket::SocketError err)
{
	QString strError =
		"Error: " + (err == QAbstractSocket::HostNotFoundError ?
			"The host was not found." :
			err == QAbstractSocket::RemoteHostClosedError ?
			"The remote host is closed." :
			err == QAbstractSocket::ConnectionRefusedError ?
			"The connection was refused." :
			QString(socket->errorString())
			);

	qDebug() << strError;
}

void Als::slotSendToServer()
{
	//Посылаем команды, как подключились к сокету
	slotSendReadStateCommand();
	slotSendWriteTimeCommand();
}

void Als::slotConnected()
{
	emit goodConnect();
}
