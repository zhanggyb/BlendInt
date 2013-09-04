#include "mainwindow.h"
#include <QApplication>

#include <Cpp/Events.hpp>

int main(int argc, char *argv[])
{
    Cpp::Events::ProcessInit processInit;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
