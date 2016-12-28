#include "roombaodometry.h"
#include "roombaserial.h"
#include <QtCore/QDebug>
#include <QtMath>

RoombaOdometry::RoombaOdometry()
{
    resetOdometyData();
    angular_coordinate = 0;
}

RoombaOdometry::~RoombaOdometry()
{
    resetOdometyData();
}

void RoombaOdometry::setRoombaSerial(roombaSerial *rs)
{
    myRoombaSerial = rs;
}

void RoombaOdometry::roombaSerialDataUpdated()
{
    // if previous encorder data is initialized
    if(priviousLeftRoteryEncorderCount != 0 && priviousRignthRoteryEncorderCount != 0)
    {
        //process when Encoder countover
        if((myRoombaSerial->fetchPacket(EncoderCountsLeft) - priviousLeftRoteryEncorderCount) > 30000)
        {
            qDebug("(myRoombaSerial->fetchPacket(EncoderCountsLeft) - priviousLeftRoteryEncorderCount) > 30000");
            qDebug() << QString::number(myRoombaSerial->fetchPacket(EncoderCountsLeft) - priviousLeftRoteryEncorderCount);
            priviousLeftRoteryEncorderCount += roteryEncorderCountMax;
        }
        else if((myRoombaSerial->fetchPacket(EncoderCountsLeft) - priviousLeftRoteryEncorderCount) < -30000)
        {
            qDebug("(myRoombaSerial->fetchPacket(EncoderCountsLeft) - priviousLeftRoteryEncorderCount) < -30000");
            priviousLeftRoteryEncorderCount -= roteryEncorderCountMax;
        }

        if((myRoombaSerial->fetchPacket(EncoderCountsRight) - priviousRignthRoteryEncorderCount) > 30000)
        {
            qDebug("(myRoombaSerial->fetchPacket(EncoderCountsRight) - priviousRignthRoteryEncorderCount) > 30000");
            qDebug() << QString::number(myRoombaSerial->fetchPacket(EncoderCountsRight) - priviousRignthRoteryEncorderCount);
            priviousRignthRoteryEncorderCount += roteryEncorderCountMax;
        }
        else if((myRoombaSerial->fetchPacket(EncoderCountsRight) - priviousRignthRoteryEncorderCount) < -30000)
        {
            qDebug("(myRoombaSerial->fetchPacket(EncoderCountsRight) - priviousRignthRoteryEncorderCount) < -30000");
            priviousRignthRoteryEncorderCount -= roteryEncorderCountMax;
        }

        leftVelocity =  (myRoombaSerial->fetchPacket(EncoderCountsLeft) - priviousLeftRoteryEncorderCount) / encorderCountPerRev * DistancePerRotate / samplingInterval;
        rightVerocity =  (myRoombaSerial->fetchPacket(EncoderCountsRight) - priviousRignthRoteryEncorderCount) / encorderCountPerRev * DistancePerRotate / samplingInterval;
        verocity = (leftVelocity + rightVerocity) / 2;
        angularVelocity = (leftVelocity - rightVerocity) / roombaAxleLenght;

        double tmpLeftDest = (myRoombaSerial->fetchPacket(EncoderCountsLeft) - priviousLeftRoteryEncorderCount) / encorderCountPerRev * DistancePerRotate;
        double tmpRightDest = (myRoombaSerial->fetchPacket(EncoderCountsRight) - priviousRignthRoteryEncorderCount) / encorderCountPerRev * DistancePerRotate;
        if(tmpLeftDest > -1000 && tmpLeftDest < 1000 && tmpRightDest > -1000 && tmpRightDest < 1000)
        {
            double tmpVelocity = (tmpLeftDest + tmpRightDest) / 2;
            x_coordinate += tmpVelocity * qCos(angular_coordinate);
            y_coordinate += tmpVelocity * qSin(angular_coordinate);
            angular_coordinate -= (tmpLeftDest - tmpRightDest) / roombaAxleLenght;

            if(angular_coordinate < 0)
            {
                angular_coordinate += M_PI * 2;
            }
            while(angular_coordinate > M_PI * 2)
            {
                angular_coordinate -= M_PI * 2;
            }


        }

        emit odometryDataChanged();
    }
    priviousLeftRoteryEncorderCount = myRoombaSerial->fetchPacket(EncoderCountsLeft);
    priviousRignthRoteryEncorderCount = myRoombaSerial->fetchPacket(EncoderCountsRight);
}

void RoombaOdometry::resetOdometyData()
{
    x_coordinate = 0;
    y_coordinate = 0;
    angular_coordinate = 0;

    leftVelocity = 0;
    rightVerocity = 0;
    verocity = 0;

    angularVelocity = 0;
}

double RoombaOdometry::getXCoordinate()
{
    return x_coordinate;
}
double RoombaOdometry::getYCoordinate()
{
    return y_coordinate;
}
double RoombaOdometry::getAngularCoordinate()
{
    return angular_coordinate;
}
double RoombaOdometry::getLeftVelocity()
{
    return leftVelocity;
}
double RoombaOdometry::getRightVerocity()
{
    return rightVerocity;
}
double RoombaOdometry::getVerocity()
{
    return verocity;
}
double RoombaOdometry::getAngularVelocity()
{
    return angularVelocity;
}

