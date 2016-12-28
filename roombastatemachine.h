#ifndef ROOMBASTATEMACHINE_H
#define ROOMBASTATEMACHINE_H

#include <QtCore>
#include <QVector>
#include <QPoint>

class RoombaOdometry;
class RoombaMoveHandler;
class roombaStateMachine;
class roombaSerial;

class RoombaStateMachine : QObject
{
    Q_OBJECT
public:
    RoombaStateMachine();
    ~RoombaStateMachine();
    void addTargetPoints(QVector<QPoint> *qp);
    void startMoving();
    void resetTask();
    void setRoombaOdometory(RoombaOdometry* ro);
    void setroombaSerial(roombaSerial* rs);

    enum state{idle, moving, manualControl, resetting};

private:
    RoombaOdometry* myRoombaOdomety;
    RoombaMoveHandler* myRoombaMoveHandler;
    QVector<QPoint>* myTaskQue;
    roombaSerial* myroombaSerial;
    state currentState;


signals:

public slots:
    void process();

};

class RoombaMoveHandler : QObject
{
    Q_OBJECT
public:
    RoombaMoveHandler();
    void setRoombaOdometory(RoombaOdometry* ro);
    void setroombaSerial(roombaSerial* rs);
    void moveToPoint(int x, int y);
    double calcAngleToPoint(int fromX, int fromY, int toX, int toY);
    double calcDistanceToPoint(int fromX, int fromY, int toX, int toY);
    bool isMoveing();
    void process();

    //action of moveToPoint
    // 1. turn roomba's head to destination
    // 2. go starigh to destination
    enum moveState{idle, turning, goStraight};
    moveState currentState;
private:
    RoombaOdometry* myRoombaOdomety;
    roombaSerial* myRoombaSerial;
    int destX, destY;

    double targetRadius, targetDistance;
    double turnedRadius, movedDistance;
    double initialRadius, initialDistance;
};
#endif // ROOMBASTATEMACHINE_H
