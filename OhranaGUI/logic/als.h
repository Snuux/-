#ifndef ALS_H
#define ALS_H

#include <QtNetwork>
#include <QObject>
#include <QThread>

#include "controller.h"
#include "db.h"

class Als : public QObject
{
	Q_OBJECT

private:
    QTcpSocket *socket; //сокет
    int id; //ID алс
    int nextBlockSize; //сколько байт читать далее
    int currentNumController; //текущий номер контроллера
    int currentTypeController; //текущий тип контроллера

    QList<Controller*> controllers; //массив контроллеров

    void initControllers(); //инициализирует контроллеры

public:
    explicit Als(int _id, QString _ip, int _port, QObject *parent = 0);
    ~Als();
    void sendCommandToServer(QByteArray com); //отправляет определенную команду серверу

    int getId() const;
    QTcpSocket *getSocket() const;

    void wait(int millisecondsToWait); //ждем некотрое время

signals:
    void send(); //сообщение отправлено
    void goodConnect(); //успешно подключено

    //читаем новый ответ
    void readNewAnswer(int _numController, int _typeController, QByteArray answer, int bytesCount);

public slots:
    void slotReadyRead(); //когда готовы читать
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer(); //когда готовы отправлять
    void slotConnected(); //когда подключились к серверу

    void slotSendReadStateCommand(); //отправляем команду чтения состояния
    void slotSendWriteTimeCommand(); //отправляем команду чтения времени
    void slotSendClearMemoryCommand(); //отправляем команду очистки памяти
    void slotSendWriteKeyCommand(int numController, QString key); //отправляем команду добавления ключа
};

#endif // ALS_H
