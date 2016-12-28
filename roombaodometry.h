#ifndef ROOMBAODOMETRY_H
#define ROOMBAODOMETRY_H

#include <QtCore/QDebug>

#define encorderCountPerRev 508.8
// diameter of tire is 72mm, so circumference is below
#define DistancePerRotate (72 * 3.1415)

#define samplingInterval 0.015

#define roteryEncorderCountMax 65535
//235mm
#define roombaAxleLenght 235
class roombaSerial;

class RoombaOdometry : public QObject
{
    Q_OBJECT

public:
    RoombaOdometry();
    ~RoombaOdometry();
    void setRoombaSerial(roombaSerial *rs);

    //getter
    double getXCoordinate();
    double getYCoordinate();
    double getAngularCoordinate();
    double getLeftVelocity();
    double getRightVerocity();
    double getVerocity();
    double getAngularVelocity();
signals:
    void odometryDataChanged();

public slots:
    void roombaSerialDataUpdated();
    void resetOdometyData();

private:
    roombaSerial *myRoombaSerial;
    double x_coordinate,y_coordinate, angular_coordinate;
    double leftVelocity, rightVerocity, verocity;
    double angularVelocity, turningRadius;

    int priviousLeftRoteryEncorderCount, priviousRignthRoteryEncorderCount;

};

#endif // ROOMBAODOMETRY_H
