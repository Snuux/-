#include "oalarmmodel.h"

OAlarmModel::OAlarmModel()
{

}

int OAlarmModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

int OAlarmModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant OAlarmModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
    const OEvent& al = m_data[index.row()];

    //qDebug() << al.getReactTime().toString();

    switch(index.column()){
    case 0: return al.getId();
    case 1: return al.getObj().getName();
    case 2: return al.getObj().getPlace();
    case 3: return al.getTime();
    case 4: return al.getReactTime().toString("hh:mm:ss:zzz");
    case 5: return (al.getSkip() ? "Есть" : "");
    default: return QVariant();
    }
}

QVariant OAlarmModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    switch (section) {
    case 0: return "ID";
    case 1: return "Объект";
    case 2: return "Адрес";
    case 3: return "Время";
    case 4: return "Время реакции";
    case 5: return "Пропуск";
    default: return QVariant();
    }
}

void OAlarmModel::updateTable(QList<OEvent> M_data, bool isGlobal)
{
    if (isGlobal)
    {
        beginResetModel();
        m_data.clear();
        endResetModel();

        beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
        m_data = M_data;
        endInsertRows();

        qDebug() << "isGlobal1";

        return;
    }

    qDebug() << "isGlobal2";

    if (m_data.count() == 0)
    {
        beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
        m_data = M_data;
        endInsertRows();

        return;
    }

    //if (M_data.count() == m_data.count())
    //    return;

    QList<OEvent> newData;

    for (int i = 0; i < M_data.count(); i++)
        for (int j = 0; j < m_data.count(); j++)
        {
            //qDebug() << M_data[i].getId() << m_data[j].getId();
            if (M_data[i].getId() == m_data[j].getId())
            {
                //qDebug() << i << "yes";
                break;

            }

            if (j == m_data.count() - 1)
            {
                //qDebug() << i << "no";
                newData.push_back(M_data[i]);
            }
        }

    //beginResetModel();
    //m_data.clear();
    //endResetModel();

    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append(newData);
    endInsertRows();

    qDebug() << m_data.count();

    //NewAlarmDialog newAlarmDialog(0, newData);
    //newAlarmDialog.exec();

    for (int k = 0; k < newData.count(); k++)
    {
        //double time = 0;
        //QTime currentTime;
        //currentTime.secsTo(QTime::currentTime());
        //
        //QTime t;
        //t.start();

        QString m;
        m = m + "Добавлена новая тревога: \nАдрес: " + newData[k].getObj().getPlace() +
                " \nОбъект: " + newData[k].getObj().getName() +
                " \nВремя: " + newData[k].getTime().toString() +
                " \nПропуск: " + (newData[k].getSkip() ? "Есть" : "Нет");

        NewAlarmDialog *dialog = new NewAlarmDialog(0, m, newData[k].getId());
        dialog->setWindowModality(Qt::NonModal);
        dialog->show();
        dialogs.push_back(dialog);

        //QMessageBox msg;
        //msg.setText("Новая тревога");
        //msg.setInformativeText(m);
        //msg.setWindowModality(Qt::NonModal);
        //
        //int ret = msg.exec();
        //msg.hide();
        //msg.show();
        //
        //QAbstractButton *but = msg.addButton(tr("Ok"), QMessageBox::ActionRole);
        //
        //time += ((double) t.elapsed());
        //
        //if (msg.clickedButton() == but)
        //{
        //    BdQueries::setAlarmReactionTime(newData[k].getId(), QTime(0, 0, 0).addMSecs(time)); //TODO
        //}
        //
        //qDebug() << t.toString();
        //
        //t.restart();
    }

    qDebug() << "After: m_datacount: " << m_data.count() << "   M_datacount: " << M_data.count();
}


/*void OAlarmModel::sort(int column, Qt::SortOrder order)
{
    if (order == Qt::AscendingOrder)
        std::sort(m_data.begin(), m_data.end(), std::less<OEvent>());
    else
        std::sort(m_data.begin(), m_data.end(), std::greater<OEvent>());
}
*/
