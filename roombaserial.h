#ifndef ROOMBASERIAL_H
#define ROOMBASERIAL_H


#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QtCore/QDebug>

#include "sensorpacketproperty.h"

class roombaSerial : QSerialPort
{
    Q_OBJECT
public:
    roombaSerial();
    ~roombaSerial();

    void setPort(QSerialPortInfo info);
    void setRequestDataList(QByteArray list);
    void startStreamData();
    void stopStreamData();
    void writeData(QByteArray data);
    int fetchPacket(int id);
    void setRoombaSpeed(short reqLeftSpped, short reqRightSpped);
    void writeRoombaSpeed(short reqLeftSpped, short reqRightSpped);
    void setSafeMode();

signals:
    void streamDataReady();

private slots:
    void fetchRequestResult();

private:
    QByteArray requestDataList;
    QByteArray receivedDataBuffer;
    sensorPacketData packetData;
    sensorPacketProperty packetProperty;
    bool checkDataLength(QByteArray byteData);
    int getExpectedDataLength();
    bool isValidParity(QByteArray data);

    QTimer fetchTimer;
    //for one packet data buffer(fetchRequestResult)
    QByteArray tmpBuffer;
    //this is request data list
    QByteArray requestedList;
    long long int dataCount;

    QFile *file;
    QTextStream *out;
    QTime checkIntervalTimer;
    short leftSpeed, rightSpeed;
};
#endif // ROOMBASERIAL_H
