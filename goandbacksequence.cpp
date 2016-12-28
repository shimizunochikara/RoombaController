#include "goandbacksequence.h"
#include "roombaserial.h"
#include <QtCore/QDebug>

goAndBackSequence::goAndBackSequence()
{
    isRoombaOnAutoSequence = 0;

    previousLeftRoteryEncoderCount = -100000;
    previousRightRoteryEncoderCount = -100000;
    accumulatedLeftRoteryEncoderCount = 0;
    accumulatedRightRoteryEncoderCount = 0;

    leftBumper = 0;
    rightBumper = 0;
}
void goAndBackSequence::setRoombaSerial(roombaSerial* rs)
{
    myRoombaSerial = rs;
}

void goAndBackSequence::setRoteryEncoderCount(int left, int right)
{
    //initialize
    if(previousLeftRoteryEncoderCount == -100000)
    {
        previousLeftRoteryEncoderCount = left;
    }
    if(previousRightRoteryEncoderCount == -100000)
    {
        previousRightRoteryEncoderCount = right;
    }

    accumulatedLeftRoteryEncoderCount += left - previousLeftRoteryEncoderCount;
    accumulatedRightRoteryEncoderCount += right - previousRightRoteryEncoderCount;
    previousLeftRoteryEncoderCount = left;
    previousRightRoteryEncoderCount = right;
}

void goAndBackSequence::setBumper(int left, int right)
{
    leftBumper = left;
    rightBumper = right;
}

void goAndBackSequence::run()
{
    //update Previous Rotery encoder count
    qDebug("startProcess");
    emit updateVelocity(200, 200);
    while((leftBumper == 0) && (rightBumper == 0)){
    }
    accumulatedLeftRoteryEncoderCount = 0;
    emit updateVelocity(50, -50);
    while(accumulatedLeftRoteryEncoderCount < TurnRoteryEncoderCount){
    }
    emit updateVelocity(200, 200);
    while((leftBumper == 0) && (rightBumper == 0)){
    }
    emit updateVelocity(0, 0);
    qDebug("endProcess");
}
