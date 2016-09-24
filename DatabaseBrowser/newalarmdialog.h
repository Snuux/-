#ifndef NEWALARMDIALOG_H
#define NEWALARMDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QDialog>
#include <QDebug>

#include "ui_newalarmdialog.h"
#include "oevent.h"
#include "bdqueries.h"

#include <windows.h>

class NewAlarmDialog : public QDialog
{
    Q_OBJECT

public:
    NewAlarmDialog(QWidget *parent = 0);
    NewAlarmDialog(QWidget *parent, QString Msg, int id);
    ~NewAlarmDialog();

private:
    OEvent ev;
    QString msg;
    QTime timer;
    double time;

    Ui::NewAlarmDialogUi ui;

private slots:
    void on_okButton_clicked();
};

#endif // NEWALARMDIALOG_H
