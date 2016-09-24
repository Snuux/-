#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    settings = new QSettings("settings.conf", QSettings::IniFormat); //инициализируем настройки

	timer1Interval.setHMS(0, 2, 0); //Время обновления состояния и времени по умолчанию
	timer2Interval.setHMS(3, 0, 0);

	readSettings(); //загружаем настройки

	//подключаем кнопку подключения к меню
	connect(ui->action, &QAction::triggered, this, &MainWindow::slotAddConnection);
	//подключаем кнопку обновления таблицы к updateAls
	connect(ui->action_2, &QAction::triggered, this, &MainWindow::updateAlsTable);
	//подключаем кнопку выхода
	connect(ui->action_3, &QAction::triggered, this, &MainWindow::close);
	//подключаем кнопку настроек
	connect(ui->action_7, &QAction::triggered, this, &MainWindow::slotChangeSettings);

	timer1.setParent(this);
	timer2.setParent(this);
	connect(&timer1, SIGNAL(timeout()), this, SLOT(updateTimer1())); //таймер для обновления состояния
	connect(&timer2, SIGNAL(timeout()), this, SLOT(updateTimer2())); //таймер для обновления времени

	timer1.start(QTime(0, 0).secsTo(settings->value("timer1Interval", timer1Interval).toTime()) * 1000);
	timer2.start(QTime(0, 0).secsTo(settings->value("timer2Interval", timer2Interval).toTime()) * 1000);
}

MainWindow::~MainWindow()
{
	delete ui;
	for (int i = 0; i < allAls.count(); i++)
		delete allAls[i];

	delete settings;
}

void MainWindow::slotAddConnection()
{
	QSqlConnectionDialog dialog(this); //создаем окно подключения к БД
	if (dialog.exec() != QDialog::Accepted)
		return;

	QSqlError err = addConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
		dialog.userName(), dialog.password(), dialog.port());
	if (err.type() != QSqlError::NoError)
		QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
			"opening the connection: ") + err.text());
	//инициализируем базу данных в классе DB
	DB::name = dialog.databaseName();
	DB::db = QSqlDatabase::database(DB::name);

	//инициализируем модели таблиц алс и контроллера
	alsModel = new QSqlTableModel(ui->tableView, DB::db);
	controllerModel = new ControllerModel();

	QString str = "Успешно подключено к базе данных '";
	str += DB::name.append("'.");
	toConsole(str);

	//настраиваем таблицу алс
	updateAlsTable();
}

void MainWindow::slotChangeSettings()
{
	SettingsDialog dialog(this); //создаем окно настроек
	if (dialog.exec() != QDialog::Accepted) //после нажатия ОК
		return;

	settings->setValue("timer1Interval", dialog.timeState()); //устанавливаем значения в файл настроек из полей
	settings->setValue("timer2Interval", dialog.timeTime());

	//получаем время в секундах
	int time1 = QTime(0, 0).secsTo(settings->value("timer1Interval", timer1Interval).toTime());
	int time2 = QTime(0, 0).secsTo(settings->value("timer2Interval", timer2Interval).toTime());

	timer1.setInterval(time1 * 1000); //обновляем таймеры (умножаем на 1000 потому что в мсек)
	timer2.setInterval(time2 * 1000);

    //Добавляем ключ
    if (DB::db.isOpen() && dialog.numController() != 0 && !dialog.key().isEmpty())
    {
        for (int i = 0; i < allAls.count(); i++)
        {
            allAls[i]->slotSendWriteKeyCommand(dialog.numController(), dialog.key());
        }
    }
}

void MainWindow::toConsole(QString str)
{
	QString a = "[";
	a += QTime::currentTime().toString("HH:mm:ss").append("] ");

	ui->plainTextEdit->appendPlainText(a + str); //выводим в консоль
}

QSqlError MainWindow::addConnection(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port)
{
	QSqlError err;
	QSqlDatabase db = QSqlDatabase::addDatabase(driver, QString(dbName));
	db.setDatabaseName(dbName);
	db.setHostName(host);
	db.setPort(port);

	if (!db.open(user, passwd)) {
		err = db.lastError();
		db = QSqlDatabase();
		QSqlDatabase::removeDatabase(QString(dbName));
	}

	return err;
}

void MainWindow::readSettings()
{
	//загружаем настройки
	settings->beginGroup("MainWindow");
    resize(settings->value("size", QSize(800, 500)).toSize());
	move(settings->value("pos", QPoint(200, 200)).toPoint());
	settings->endGroup();

	timer1Interval = settings->value("timer1Interval", timer1Interval).toTime();
	timer2Interval = settings->value("timer1Interval", timer2Interval).toTime();
}

void MainWindow::writeSettings()
{
	//сохраняем настройки при выходе
	settings->beginGroup("MainWindow");
	settings->setValue("size", size());
	settings->setValue("pos", pos());
	settings->endGroup();
}

void MainWindow::closeEvent(QCloseEvent *bar)
{
	writeSettings(); //сохраняем настройки при выходе
}

void MainWindow::updateAlsTable()
{
	if (!DB::db.isOpen())
		return;

	alsModel->setTable("ohrana.als"); //получаем полную таблицу из БД
	alsModel->select(); //выполняем запрос

	alsModel->setHeaderData(0, Qt::Horizontal, tr("№")); //устанавливаем имя колонок
	alsModel->setHeaderData(1, Qt::Horizontal, tr("IP"));
	alsModel->setHeaderData(2, Qt::Horizontal, tr("Порт"));
	alsModel->removeColumn(3); //удаляем ненужные столбцы
	alsModel->removeColumn(3);

	ui->tableView->setModel(alsModel);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->tableView->resizeRowsToContents();
	ui->tableView->resizeColumnsToContents();

	QString str = "Получено ";
	str = str.append(QString::number(alsModel->rowCount())).append(" элемент(а)(ов) из таблицы 'ohrana.als'");
	toConsole(str);

	//показываем контроллеры для первого алс
    if (alsModel->rowCount() > 0)
        on_tableView_clicked(alsModel->index(0, 0));

	//настраиваем логику
	setupLogic();
}

void MainWindow::updateControllersTable(int alsId)
{
	controllerModel->updateTable(DB::getFullControllersForAls(alsId)); //получаем контроллеры из БД

	ui->tableView_2->setModel(controllerModel);
	ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->tableView_2->resizeRowsToContents();
    ui->tableView_2->resizeColumnsToContents();
}

QList<unsigned char> MainWindow::getLastStateFromController(int controllerId)
{
    QList<unsigned char> ret;

    for (int i = 0; i < controllerModel->rowCount(); i++)
    {
        if (ui->tableView_2->model()->data(controllerModel->index(i, 0, QModelIndex())).toInt() == controllerId)
        {
            QString t1 = ui->tableView_2->model()->data(controllerModel->index(i, 2, QModelIndex())).toString();
            QString t2 = ui->tableView_2->model()->data(controllerModel->index(i, 3, QModelIndex())).toString();
            ret.push_back(t1.toInt(0, 2));
            ret.push_back(t2.toInt(0, 2));
        }
    }

    return ret;
}

void MainWindow::setupLogic()
{
	//создаем алс

	for (int i = 0; i < alsModel->rowCount(); i++)
	{
		Als *a = new Als(alsModel->data(alsModel->index(i, 0)).toInt(), //id
			alsModel->data(alsModel->index(i, 1)).toString(), //ip
			alsModel->data(alsModel->index(i, 2)).toInt(), this); //port

		connect(ui->action_4, SIGNAL(triggered()), a, SLOT(slotSendReadStateCommand()));
        connect(ui->action_5, SIGNAL(triggered()), a, SLOT(slotSendWriteTimeCommand()));
        connect(ui->action_6, SIGNAL(triggered()), a, SLOT(slotSendClearMemoryCommand()));

		allAls.push_back(a);
	}

	QString str = "Запущен опрос контроллеров подключенных к Алс с номерами: { ";
	for (int i = 0; i < alsModel->rowCount(); i++)
		str += QString::number(alsModel->data(alsModel->index(i, 0)).toInt()).append(",");
	toConsole(str + " }");
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
	//достаем из таблицы номер алс, и настраиваем таблицу контроллеров
	updateControllersTable(alsModel->data(alsModel->index(index.row(), 0)).toInt());

	//выделяем целую строку (вместо 1 ячейки)
	ui->tableView->selectRow(index.row());
}

void MainWindow::updateTimer1()
{
	qDebug() << "timer1";
	for (int i = 0; i < allAls.count(); i++)
	{
		allAls[i]->slotSendReadStateCommand();
	}
}

void MainWindow::updateTimer2()
{
	qDebug() << "timer2";
	for (int i = 0; i < allAls.count(); i++)
	{
		allAls[i]->slotSendWriteTimeCommand();
	}
}


