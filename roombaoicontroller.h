#ifndef ROOMBAOICONTROLLER_H
#define ROOMBAOICONTROLLER_H

#include <QtCore>

class MainWindow;
class roombaSerial;
class dataProcessor;
class RoombaOdometry;
class RoombaStateMachine;

class roombaOiController : public QObject
{
    Q_OBJECT

public:
    roombaOiController();
    ~roombaOiController();
private:
    MainWindow* w;
    roombaSerial* roomba;
    dataProcessor* dp;
    RoombaOdometry* roombaOdometry;
    RoombaStateMachine* myRoombaStateMachine;
};

#endif // ROOMBAOICONTROLLER_H
