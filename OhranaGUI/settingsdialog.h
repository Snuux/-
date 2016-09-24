#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTime>

#include "mainwindow.h"

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

	QTime timeState();
	QTime timeTime();
    QString key();
    int numController();

private:
	Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
