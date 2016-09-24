#include "browser.h"

#include <QtCore>
#include <QtWidgets>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow mainWin;
    mainWin.setWindowTitle(QObject::tr("ПО Диспетчер"));
    mainWin.statusBar()->setSizeGripEnabled(true);
    //mainWin.set(1200, 700);

    Browser browser(&mainWin);
    mainWin.setCentralWidget(&browser);

    QMenu *fileMenu = mainWin.menuBar()->addMenu(QObject::tr("&Файл"));
    fileMenu->addAction(QObject::tr("Добавить &подключение..."), &browser, SLOT(addConnection()));
    fileMenu->addSeparator();
    fileMenu->addAction(QObject::tr("&Выход"), &app, SLOT(quit()));

    QMenu *testMenu = mainWin.menuBar()->addMenu(QObject::tr("&Тестирование"));
    testMenu->addAction(QObject::tr("Добавить &тревогу..."), &browser, SLOT(insertAlarm()));

    QMenu *helpMenu = mainWin.menuBar()->addMenu(QObject::tr("&Дополнительно"));
    helpMenu->addAction(QObject::tr("О фреймворке Qt"), qApp, SLOT(aboutQt()));

    mainWin.show();
    mainWin.statusBar()->showMessage("Подключение к базе данных");

    return app.exec();
}
