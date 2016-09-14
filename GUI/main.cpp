#include "debugwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv); // initialize Qt application
	Gbmu::Gb gb; // create a Gb object
	DebugWindow w(0, &gb); // create a DebugWindow (debugger)

	w.show(); // show window in Qt application

	return a.exec(); // start Qt application
}
