#include <QApplication>

#include "main.h"

MainWindow* window;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("OhranaGUI");
	QCoreApplication::setOrganizationDomain("google.com");
	QCoreApplication::setApplicationName("OhranaGUI");

    window = new MainWindow();
    window->show();

	return a.exec();
}
