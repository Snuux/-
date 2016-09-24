#ifndef BROWSER_H
#define BROWSER_H

#include "qsqlconnectiondialog.h"
#include "bdqueries.h"

#include <QWidget>
#include <QtSql>
#include <QDebug>
#include <QString>
#include <QSortFilterProxyModel>
#include <QBasicTimer>
#include <QDate>

#include "bdqueries.h"

#include "oobjectmodel.h"
#include "oeventmodel.h"
#include "oohranamodel.h"
#include "oalarmmodel.h"
#include "osearchmodel.h"

#include "oeventdelegate.h"
#include "oohranadelegate.h"
#include "osearchdelegate.h"

#include "ui_browserwidget.h"

#include "oevent.h"

class Browser : public QWidget, private Ui::Browser
{
    Q_OBJECT
public:
    explicit Browser(QWidget *parent = 0);
    virtual ~Browser();

signals:

public slots:
    void addConnection();
    void searchButtonClick();
    void insertAlarm();

private:
    OobjectModel m_oobjectmodel;
    OEventModel m_oeventmodel;
    OOhranaModel m_oohranamodel;
    OAlarmModel m_oalarmmodel;
    OSearchModel m_osearchmodel;

    QSortFilterProxyModel m_oobjectproxymodel;
    QSortFilterProxyModel m_oeventproxymodel;
    QSortFilterProxyModel m_oohranaproxymodel;
    QSortFilterProxyModel m_oalarmproxymodel;
    QSortFilterProxyModel m_osearchproxymodel;

    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                  const QString &user, const QString &passwd, int port = -1);

    void updateTables();
    QBasicTimer timer;
    bool connect;

    OEvent getNewOhran();

    // QObject interface
protected:
    void timerEvent(QTimerEvent *);
private slots:
    void on_checkBox_toggled(bool checked);
    void on_dateEdit_3_dateChanged(const QDate &date);
    void on_checkBox_2_toggled(bool checked);
};

#endif // BROWSER_H
