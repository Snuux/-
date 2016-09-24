#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	MainWindow * a = (MainWindow*)parentWidget();

	QTime time1;
	QTime time2;

	//читаем сохраненные данные
	time1 = a->settings->value("timer1Interval", a->timer1Interval).toTime();
	time2 = a->settings->value("timer2Interval", a->timer2Interval).toTime();

	//устанавливаем значения из прочитанных сохр. данных
	ui->timeEdit->setTime(time1);
	ui->timeEdit_2->setTime(time2);
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

QTime SettingsDialog::timeState()
{
	return ui->timeEdit->time();
}

QTime SettingsDialog::timeTime()
{
    return ui->timeEdit_2->time();
}

QString SettingsDialog::key()
{
    return ui->lineEdit->text();
}

int SettingsDialog::numController()
{
    return ui->lineEdit_2->text().toInt();
}
