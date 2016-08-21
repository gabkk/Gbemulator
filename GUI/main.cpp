#include "debugwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gbmu::Gb gb;
    DebugWindow w(0, &gb);

    w.show();

    return a.exec();
}
