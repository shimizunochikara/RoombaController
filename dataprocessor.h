#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QtCore>
#include "sensorpacketproperty.h"

class roombaSerial;

class dataProcessor : public QObject
{
    Q_OBJECT
public:
    dataProcessor();
    ~dataProcessor();
    void setRoombaSerial(roombaSerial* _rs);
    unsigned int getCurrentBattery();
    unsigned int getBatteryCapacity();
    unsigned int getOpenInterfaceMode();
    unsigned int getEncoderCountsLeft();
    unsigned int getEncoderCountsRight();
    short getLightBumpCenterLeft();
    short getLightBumpCenterRight();
    short getDirtDetect();

signals:

public slots:
    void dataReceived();

private:
    sensorPacketProperty packetProperty;
    sensorPacketData lastPacketData;
    roombaSerial* rs;
};

#endif // DATAPROCESSOR_H
