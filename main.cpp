#include "mainMenu.h"
#include <QApplication>
#include <QScreen>
#include <QSize>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainMenu w;
    w.show();

    return a.exec();
}
