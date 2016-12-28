
#include <QApplication>
#include <QtCore/QDebug>

#include "roombaoicontroller.h"

#define DATASIZE 64

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    roombaOiController roc;
    return a.exec();
}
