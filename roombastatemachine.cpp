#include "roombastatemachine.h"
#include "roombaodometry.h"
#include "roombaserial.h"
#include <QtMath>

RoombaStateMachine::RoombaStateMachine()
{
    myRoombaMoveHandler = new RoombaMoveHandler();
    myTaskQue = new QVector<QPoint>();
}

RoombaStateMachine::~RoombaStateMachine()
{
    delete myRoombaMoveHandler;
    delete myTaskQue;
}

void RoombaStateMachine::addTargetPoints(QVector<QPoint> *qp)
{
    for(int i = 1; i < qp->count(); i++)
    {
        myTaskQue->append(qp->at(i));
        qDebug("myTaskQue->append(qp->at(i));");
    }
}

void RoombaStateMachine::startMoving()
{
    currentState = moving;
    qDebug("startMoving");
}

void RoombaStateMachine::resetTask()
{
    currentState = resetting;
    for(int i = 0; i < myTaskQue->count(); i++)
    {
        myTaskQue->removeLast();
        qDebug("removeList");
    }
}

void RoombaStateMachine::process()
{

    switch(currentState)
    {
    case idle:
        break;
    case moving:
        if(!myRoombaMoveHandler->isMoveing())
        {
            if(myTaskQue->count() > 0)
            {
                myRoombaMoveHandler->moveToPoint(myTaskQue->first().x(), myTaskQue->first().y());
                myTaskQue->removeFirst();
                currentState = moving;
            }
            else
            {
                currentState = idle;
            }
        }
        myRoombaMoveHandler->process();
        break;
    case resetting:
        qDebug("resetting");
        currentState = idle;
        break;
    default:
        break;
    }
}

void RoombaStateMachine::setRoombaOdometory(RoombaOdometry* ro)
{
    myRoombaOdomety = ro;
    myRoombaMoveHandler->setRoombaOdometory(ro);
}

void RoombaStateMachine::setroombaSerial(roombaSerial* rs)
{
    myroombaSerial = rs;
    myRoombaMoveHandler->setroombaSerial(rs);
}

/****************************************************/
/* RoombaMoveHandler class                          */
/****************************************************/
RoombaMoveHandler::RoombaMoveHandler()
{
    currentState = idle;
}

void RoombaMoveHandler::moveToPoint(int x, int y)
{
    qDebug("***moveToPoint: %d, %d, *** turning", x, y);
    destX = x;
    destY = y;
    turnedRadius = 0;
    movedDistance = 0;

    targetRadius = calcAngleToPoint(myRoombaOdomety->getXCoordinate(), myRoombaOdomety->getYCoordinate(), x, y);
    qDebug("targetRadius = %lf", targetRadius);
    targetDistance = calcDistanceToPoint(myRoombaOdomety->getXCoordinate(), myRoombaOdomety->getYCoordinate(), x, y);
    qDebug("targetDistance = %lf", targetDistance);
    myRoombaSerial->setSafeMode();
    if(targetRadius > 0)
    {
        myRoombaSerial->setRoombaSpeed(-50, 50);
    }
    else
    {
        myRoombaSerial->setRoombaSpeed(50, -50);
    }
    currentState = turning;
}

void RoombaMoveHandler::setRoombaOdometory(RoombaOdometry* ro){
    myRoombaOdomety = ro;
}


double RoombaMoveHandler::calcAngleToPoint(int fromX, int fromY, int toX, int toY)
{
    qDebug("calcAngleToPoint(fromX: %d, fromY: %d, toX : %d, toY : %d) = %lf", fromX, fromY, toX, toY,qAtan2(toY - fromY, toX - fromX) );
    double tempTarget;
    double currentAngle = myRoombaOdomety->getAngularCoordinate();

    // change value to absolute(theta = -1 -> 5.28)
    if(qAtan2(toY - fromY, toX - fromX) >= 0)
    {
        tempTarget = qAtan2(toY - fromY, toX - fromX);
    }
    else
    {
        tempTarget = M_PI * 2 + qAtan2(toY - fromY, toX - fromX);
    }
    qDebug("calcAngleToPoint: tempTarget = %lf, currentAngle = %lf", tempTarget, currentAngle);
    // calculate turn angle
    // The first quadrant
    if(M_PI / 2 > currentAngle)
    {
        // The first quadrant
        if(M_PI / 2 > tempTarget)
        {
            qDebug("pattern 1 - 1");
            //turn left or right
            return tempTarget - currentAngle;
        }
        // The second quadrant
        else if(M_PI / 2 <= tempTarget && tempTarget < M_PI)
        {
            qDebug("pattern 1 - 2");
            // always turn left
            return tempTarget - currentAngle;
        }
        // The third quadrant
        else if(M_PI <= tempTarget && M_PI < tempTarget * 3 /2)
        {
            if(currentAngle + M_PI >= tempTarget)
            {
                qDebug("pattern 1 - 3 - 1");
                //turn left
                return tempTarget - currentAngle;
            }
            else
            {
                qDebug("pattern 1 - 3 - 2");
                //turn right, stride 0 angle
                return - ((M_PI * 2 - tempTarget) + currentAngle);
            }
        }
        //fourth quadrant
        else
        {
            qDebug("pattern 1 - 4");
            //right turn
            return - (currentAngle + (M_PI * 2 - tempTarget));
        }
    }
    // The second quadrant
    else if(M_PI / 2 <= currentAngle && currentAngle < M_PI)
    {
        // The first quadrant
        if(M_PI / 2 > tempTarget)
        {
            qDebug("pattern 2 - 1");
            //always turn right
            return tempTarget - currentAngle;
        }
        // The second quadrant
        else if(M_PI / 2 <= tempTarget && tempTarget <M_PI)
        {
            qDebug("pattern 2 - 2");
            //turn left or right
            return tempTarget - currentAngle;
        }
        // The third quadrant
        else if(M_PI <= tempTarget && M_PI < tempTarget * 3 /2)
        {
            qDebug("pattern 2 - 3");
            //turn left
            return tempTarget - currentAngle;
        }
        //fourth quadrant
        else
        {
            if(currentAngle + M_PI <= tempTarget)
            {
                qDebug("pattern 2 - 4 - 1");
                //turn left
                return tempTarget - currentAngle;
            }
            else
            {
                qDebug("pattern 2 - 4 - 2");
                //turn right, stride 0 angle
                return - ((M_PI * 2 - tempTarget) + currentAngle);
            }
        }

    }
    // The third quadrant
    else if(M_PI  <= currentAngle && currentAngle < M_PI * 3 / 2)
    {
        // The first quadrant
        if(M_PI / 2 > tempTarget)
        {
            if(M_PI - currentAngle >= tempTarget)
            {
                qDebug("pattern 3 - 1 - 1");
                //turn left
                return tempTarget - currentAngle;
            }
            else
            {
                qDebug("pattern 3 - 1 - 2");
                //turn right, stride 0 angle
                return - ((M_PI * 2 - currentAngle) + tempTarget);
            }

        }
        // The second quadrant
        else if(M_PI / 2 <= tempTarget && tempTarget < M_PI)
        {
            qDebug("pattern 3 - 2");
            // always turn right
            return tempTarget-  currentAngle;
        }
        // The third quadrant
        else if(M_PI <= tempTarget && M_PI < tempTarget * 3 /2)
        {
            qDebug("pattern 3 - 3");
            //turn left or right
            return tempTarget - currentAngle;
        }
        //fourth quadrant
        else
        {
            qDebug("pattern 3 - 4");
            //turn left
            return tempTarget - currentAngle;
        }
    }
    else
    {
        // The first quadrant
        if(M_PI / 2 > tempTarget)
        {
            qDebug("pattern 4 - 1");
            //always turn left
            return (M_PI * 2 - currentAngle) + tempTarget;
        }
        // The second quadrant
        else if(M_PI / 2 <= tempTarget && tempTarget < M_PI)
        {
            if(currentAngle - M_PI <= tempTarget)
            {
                qDebug("pattern 4 - 2 - 1");
                //turn left
                return  currentAngle - tempTarget;
            }
            else
            {
                qDebug("pattern 4 - 2 - 2");
                //turn left, stride 0 angle
                return (M_PI * 2 - currentAngle) + tempTarget;
            }
        }
        // The third quadrant
        else if(M_PI <= tempTarget && M_PI < tempTarget * 3 /2)
        {
            qDebug("pattern 4 - 3");
            //turn right
            return tempTarget - currentAngle;
        }
        //fourth quadrant
        else
        {
            qDebug("pattern 4 - 4");
            //turn left or right
            return tempTarget - currentAngle;
        }
    }
}

double RoombaMoveHandler::calcDistanceToPoint(int fromX, int fromY, int toX, int toY)
{
    //qDebug("calcDistanceToPoint(fromX: %d, fromY: %d, toX : %d, toY : %d) = %lf", fromX, fromY, toX, toY, qSqrt( qPow(toX - fromX ,2) + qPow(toY - fromY, 2) ));
    return qSqrt( qPow(toX - fromX ,2) + qPow(toY - fromY, 2) );
}

bool RoombaMoveHandler::isMoveing()
{
    return (idle != currentState );
}

void RoombaMoveHandler::process()
{
    switch(currentState)
    {
    case idle:
        break;
    case turning:
        turnedRadius += qFabs(myRoombaOdomety->getAngularVelocity()) * samplingInterval;
        //qDebug("turnedRadius: %lf", turnedRadius);
        if(qFabs(qFabs(targetRadius) <= turnedRadius))
        {
            myRoombaSerial->setRoombaSpeed(100, 100);
            currentState = goStraight;
            qDebug("RoombaMoveHandler::goStraight");
        }
        break;
    case goStraight:
        movedDistance += qFabs(myRoombaOdomety->getVerocity()) * samplingInterval;
        //qDebug("movedDistance: %lf", movedDistance);
        if(targetDistance <= movedDistance)
        {
            myRoombaSerial->setRoombaSpeed(0, 0);
            currentState = idle;
            qDebug("RoombaMoveHandler::idle");
        }
        break;
    default:
        break;
    }
}

void RoombaMoveHandler::setroombaSerial(roombaSerial* rs)
{
    myRoombaSerial = rs;
}
