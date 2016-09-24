#include "browser.h"

Browser::Browser(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    parent->adjustSize();

    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(searchButtonClick()));

    //addConnection();

    tab_2->setFocus();

    comboBox->addItem("Все");
    comboBox->addItem("Дверь");
    comboBox->addItem("Охрана");
    comboBox->addItem("Тревога");

    connect = false;

    timer.start(15000, this);

    dateEdit_3->setDate(QDate().currentDate());
}

Browser::~Browser()
{
}

void Browser::addConnection()
{
    QSqlConnectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QSqlError err = addConnection(dialog.driverName(), dialog.databaseName(), dialog.hostName(),
                       dialog.userName(), dialog.password(), dialog.port());
    if (err.type() != QSqlError::NoError)
        QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                   "opening the connection: ") + err.text());

    updateTables();

    connect = true;
}

void Browser::searchButtonClick()
{
    tableView_5->setModel(&m_osearchproxymodel);
    m_osearchmodel.updateTable(BdQueries::getAll("", lineEdit->text(), lineEdit_2->text(), timeEdit->time(), timeEdit_2->time(), dateEdit->date(), dateEdit_2->date(), lineEdit_3->text(), comboBox->currentIndex()));

    m_osearchproxymodel.setSourceModel(&m_osearchmodel);
    tableView_5->setColumnHidden(0, true);

    tableView_5->resizeRowsToContents();
    tableView_5->resizeColumnsToContents();

    tableView_5->setSortingEnabled(true);

    m_osearchproxymodel.sort(4);
    tableView_5->sortByColumn(4, Qt::DescendingOrder);

    tableView_5->setItemDelegate(new OSearchDelegate(&m_osearchproxymodel, 7));
}

void Browser::insertAlarm()
{
    BdQueries::insertAllarm();
}

QSqlError Browser::addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port)
{
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, QString("Ohrana"));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Ohrana"));
    }

    return err;
}

void Browser::updateTables()
{
    tableView->setModel(&m_oobjectproxymodel);
    tableView_2->setModel(&m_oeventproxymodel);
    tableView_3->setModel(&m_oohranaproxymodel);
    tableView_4->setModel(&m_oalarmproxymodel);
    //tableView_5->setModel(&m_osearchproxymodel);

    m_oobjectmodel.updateTable(BdQueries::getObjects());
    m_oeventmodel.updateTable(BdQueries::getEvents(dateEdit_3->date()));
    m_oohranamodel.updateTable(BdQueries::getOhrans(dateEdit_3->date()));
    m_oalarmmodel.updateTable(BdQueries::getAlarms(dateEdit_3->date()), false);
    //m_osearchmodel.updateTable(BdQueries::getAll("", lineEdit->text(), lineEdit_2->text(), timeEdit->time(), timeEdit_2->time(), dateEdit->date(), dateEdit_2->date(), "", comboBox->currentIndex()));

    m_oeventproxymodel.setSourceModel(&m_oeventmodel);
    m_oalarmproxymodel.setSourceModel(&m_oalarmmodel);
    m_oohranaproxymodel.setSourceModel(&m_oohranamodel);
    m_oobjectproxymodel.setSourceModel(&m_oobjectmodel);
    //m_osearchproxymodel.setSourceModel(&m_osearchmodel);

    tableView->setColumnHidden(0, true);
    tableView_2->setColumnHidden(0, true);
    tableView_3->setColumnHidden(0, true);
    tableView_4->setColumnHidden(0, true);
    //tableView_5->setColumnHidden(0, true);

    tableView_2->setItemDelegate(new OEventDelegate(&m_oeventproxymodel));
    tableView_3->setItemDelegate(new OOhranaDelegate(&m_oohranaproxymodel));

    tableView->setSortingEnabled(true);
    tableView_2->setSortingEnabled(true);
    tableView_3->setSortingEnabled(true);
    tableView_4->setSortingEnabled(true);
    //tableView_5->setSortingEnabled(true);

    m_oalarmproxymodel.sort(3);
    m_oohranaproxymodel.sort(4);
    m_oeventproxymodel.sort(4);

    tableView_2->sortByColumn(4, Qt::DescendingOrder);
    tableView_3->sortByColumn(4, Qt::DescendingOrder);
    tableView_4->sortByColumn(3, Qt::DescendingOrder);

    tableView->resizeRowsToContents();
    tableView_2->resizeRowsToContents();
    tableView_3->resizeRowsToContents();
    tableView_4->resizeRowsToContents();
    //tableView_5->resizeRowsToContents();

    tableView->resizeColumnsToContents();
    tableView_2->resizeColumnsToContents();
    tableView_3->resizeColumnsToContents();
    tableView_4->resizeColumnsToContents();
    //tableView_5->resizeColumnsToContents();
}

OEvent Browser::getNewOhran()
{
}

void Browser::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        if (connect)
        {
            updateTables();
            qDebug() << "updateTables() with Timer";
        }
    } else {
        QWidget::timerEvent(event);
    }
}

void Browser::on_checkBox_toggled(bool checked)
{
    if (checked)
    {
        dateEdit_3->setDate(QDate().currentDate());
        dateEdit_3->setEnabled(false);
        m_oalarmmodel.updateTable(BdQueries::getAlarms(dateEdit_3->date()), true);
        updateTables();
    }
    else
    {
        dateEdit_3->setEnabled(true);
        m_oalarmmodel.updateTable(BdQueries::getAlarms(dateEdit_3->date()), true);
        updateTables();
    }

}

void Browser::on_dateEdit_3_dateChanged(const QDate &date)
{
    m_oalarmmodel.updateTable(BdQueries::getAlarms(dateEdit_3->date()), true);
    updateTables();
    qDebug() << "updateTables() with dateChanged";
}



void Browser::on_checkBox_2_toggled(bool checked)
{
    if (checked)
    {
        dateEdit_3->setEnabled(false);
        checkBox->setEnabled(false);
        QDate d;
        d.setDate(1752,9,14); //hack
        dateEdit_3->setDate(d);
        qDebug() << dateEdit_3->date();
        qDebug() << "updateTables() with on_checkBox_2_toggled true";
        m_oalarmmodel.updateTable(BdQueries::getAlarms(dateEdit_3->date()), true);
        updateTables();
    }
    else
    {
        dateEdit_3->setEnabled(true);
        checkBox->setEnabled(true);
        dateEdit_3->setDate(QDate().currentDate());
        qDebug() << "updateTables() with on_checkBox_2_toggled false";
        m_oalarmmodel.updateTable(BdQueries::getAlarms(dateEdit_3->date()), true);
        updateTables();
    }
}
