
#include <QApplication>
#include <QtCore/QDebug>

#include "roombaoicontroller.h"

#define DATASIZE 64

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    roombaOiController roc;
    return a.exec();
}
