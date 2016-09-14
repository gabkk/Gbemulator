#include <QApplication>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv); // initialize Qt application
	Gbmu::Gb gb; // create a Gb object

	MainWindow w(0, &gb);

	w.show(); // show main window in Qt application

	return a.exec(); // start Qt application
}
