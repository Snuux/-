#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>

#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include <QAbstractItemModel>

#include <QTimer>

#include <QSettings>

#include "qsqlconnectiondialog.h"
#include "controllermodel.h"
#include "settingsdialog.h"

#include "logic/db.h"
#include "logic/als.h"

namespace Ui {
	class MainWindow;
}

class Als;
class ControllerModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

    void toConsole(QString str); //отправляет сообщ. в консоль

    QTime timer1Interval; //задает интервал опроса оборудования
    QTime timer2Interval; //задает интервал установки времени оборудования
    QSettings *settings; //задает настройки

    void updateAlsTable(); //обновляет таблицу с алс
    void updateControllersTable(int alsId); //обновляет таблицу с контроллерами
    QList<unsigned char> getLastStateFromController(int controllerId);

private:
    Ui::MainWindow *ui; //главный ui объект
    QList<Als *> allAls; //массив als
    QTimer timer1; //таймер опроса
    QTimer timer2; //таймер установки времени

	void setupLogic(); //функция настраивает "логику", т.е. взаимосвязь с контроллером

	QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
		const QString &user, const QString &passwd, int port = -1);

    QSqlTableModel *alsModel; //алс
    ControllerModel *controllerModel; //модель для таблицы

    void readSettings(); //читает сохр. настройки
    void writeSettings(); //пишем сохр. настройки

    void closeEvent(QCloseEvent *bar); //функция при закрытии окна (сохраняем настройки)

	private slots:
    void slotAddConnection(); //добавляет соединение
    void slotChangeSettings(); //изменяет настройки
    void on_tableView_clicked(const QModelIndex &index); //вызывается при клике на таблицу

    void updateTimer1(); //обновляет состояние
    void updateTimer2(); //обновляет настройку времени
};

#endif // MAINWINDOW_H
