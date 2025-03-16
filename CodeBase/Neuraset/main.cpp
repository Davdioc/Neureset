#include <QApplication>

#include "maincontrol.h"
#include "defs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainControl main;

    return a.exec();
}
