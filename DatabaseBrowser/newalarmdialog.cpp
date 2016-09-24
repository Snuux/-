#include "newalarmdialog.h"
#include "ui_newalarmdialog.h"

//NewAlarmDialog::NewAlarmDialog(QWidget *parent)
//{
//    setParent(parent);
//    ui.setupUi(this);
//}

NewAlarmDialog::NewAlarmDialog(QWidget *parent, QString Msg, int id)
{
    setParent(parent);
    ui.setupUi(this);

    ev.setId(id);
    msg = Msg;
    timer.setHMS(0,0,0);
    time = 0;

    timer.start();

    //QObject::connect(Ui::NewAlarmDialogUi.pushButton, SIGNAL(clicked()), this, SLOT(on_okButton_clicked()));

    ui.label->setText(msg);

    Beep(2000, 1000);
}

NewAlarmDialog::~NewAlarmDialog()
{
}

void NewAlarmDialog::on_okButton_clicked()
{
    time += ((double) timer.elapsed());
    qDebug()  << "timeid:  " << QTime(0, 0, 0).addMSecs(time).toString() << ev.getId();
    BdQueries::setAlarmReactionTime(ev.getId(), QTime(0, 0, 0).addMSecs(time)); //TODO
    close();
}
