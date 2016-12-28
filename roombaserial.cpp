#include "roombaserial.h"
#include<windows.h>


#define code0 255

roombaSerial::roombaSerial()
{
    file = new QFile("serialInterval.txt");
    if (!file->open(QIODevice::WriteOnly))
    {
        qDebug("file cannot open");
        return;
    }

    out = new QTextStream(file);
    checkIntervalTimer.start();

    dataCount = 0;
    leftSpeed = 0;
    rightSpeed = 0;
    fetchTimer.setInterval(1);
    connect(&fetchTimer, SIGNAL(timeout()), this, SLOT(fetchRequestResult()));
    fetchTimer.start();

}
roombaSerial::~roombaSerial()
{
    fetchTimer.stop();
    disconnect(&fetchTimer, SIGNAL(timeout()), this, SLOT(fetchRequestResult()));
    close();
    //for destruct loging file
    delete out;
    file->close();
    delete file;

}

void roombaSerial::setPort(QSerialPortInfo info)
{

    QSerialPort::setPort(info);
    qDebug() << "Name        : " << info.portName();
    qDebug() << "Description : " << info.description();
    qDebug() << "Manufacturer: " << info.manufacturer();

    // roomba default baud rate is 115200.
    setBaudRate(115200);
    open(QIODevice::ReadWrite);

}

void roombaSerial::setRequestDataList(QByteArray list)
{
    requestDataList = list;
    packetData.setRequestDataList(list);
}

void roombaSerial::startStreamData()
{
    flush();
    connect(this, SIGNAL(readyRead()), this, SLOT(fetchRequestResult()));
    QByteArray commandByteArray;
    // start command
    commandByteArray.append(128);

    // stream command
    commandByteArray.append(148);
    // send Number of packets.
    commandByteArray.append(requestDataList.length());
    //send request data list.
    for(int i = 0; i < requestDataList.length(); i++){
        commandByteArray.append(requestDataList.at(i));
    }

    writeData(commandByteArray);

}

void roombaSerial::stopStreamData()
{
    QByteArray commandByteArray;
    //Stream pause command
    commandByteArray.append(150);
    commandByteArray.append(code0);

    // stop command
    commandByteArray.append(173);
    writeData(commandByteArray);
    disconnect(this, SIGNAL(readyRead()), this, SLOT(fetchRequestResult()));
    flush();
}


void roombaSerial::fetchRequestResult()
{
    bool isDataValid = false;

    if(QSerialPort::bytesAvailable() > 0)
    {
        receivedDataBuffer.append(QSerialPort::readAll());
    }
    //remove extra data
    if(receivedDataBuffer.length() > 0)
    {
        if(receivedDataBuffer.at(0) != 19)
        {
            *out << "head data is :" << QString::number(receivedDataBuffer.at(0));
            int i;
            for(i = 0; i < receivedDataBuffer.length(); i++)
            {
                if(receivedDataBuffer.at(i) == 19)
                {
                    i--;
                    break;
                }
            }
            receivedDataBuffer.remove(0, i + 1);
            //qDebug("remove extra data");
            *out << " ,remove extra data, removed byte:" << i + 1 << " ,expected byte: " << getExpectedDataLength() << ":"<< checkIntervalTimer.elapsed() << endl;
        }
    }
    //top of buffer data is header
    if(receivedDataBuffer.length() >= getExpectedDataLength())
    {
        //clean tmpbuffer
        tmpBuffer.remove(0, tmpBuffer.length());
        //move from serial buffer to temporary buffer.
        int i;
        for(i = 0; i < getExpectedDataLength(); i++)
        {
            tmpBuffer.append(receivedDataBuffer.at(i));
        }
        receivedDataBuffer.remove(0, getExpectedDataLength());
        //check parity
        //data is valid
        if(isValidParity(tmpBuffer))
        {
            //yes, this data is valid
            isDataValid = true;
            packetData.setArrivedData(tmpBuffer);
            emit streamDataReady();
            writeRoombaSpeed(leftSpeed,rightSpeed);
            *out << "serial data is valid:" << checkIntervalTimer.elapsed() << endl;
            dataCount ++;
        }
        //data is invalid
        else
        {
            qDebug("serial data is invalid");
            *out << "serial data is invalid:" << checkIntervalTimer.elapsed() << endl;
        }

    }
}

void roombaSerial::writeData(QByteArray data)
{
    char str;
    for(int i = 0; i < data.length(); i++){
        // cannot append 0 to QByteArray.
        if(code0 == data.at(i))
        {
            str = 0;
        }
        else
        {
            str = data.at(i);
        }
        QSerialPort::writeData(&str,sizeof(char));
    }
}

int roombaSerial::getExpectedDataLength()
{
    int dataByteLength = 0;
    //this is for stream header, data length data, and check sum
    dataByteLength += 3;
    for(int i = 0; i < requestDataList.length(); i++)
    {
        // add ID size
        dataByteLength += 1;
        dataByteLength += packetProperty.getDataSize(requestDataList.at(i));
    }
    return dataByteLength;
}

bool roombaSerial::checkDataLength(QByteArray byteData)
{
    bool isValid = false;
    int dataByteLength = getExpectedDataLength();

    if(byteData.length() == dataByteLength)
    {
        isValid = true;
    }
    else
    {
        qDebug("checkDataLength error: expect %d byte, actual %d byte", dataByteLength, byteData.length());
    }
    return isValid;
}

int roombaSerial::fetchPacket(int id)
{
    bool isPacketExisted = false;
    int targetDataByteOrder = 0;
    //for header, pakcet data length byte, and datasize that is myself byte
    targetDataByteOrder += 3;
    // search id from requestdataList
    for(int i = 0; i < requestDataList.length(); i++)
    {
        if(requestDataList.at(i) == id)
        {
            isPacketExisted = true;
            // escape from this roop before add targetDataByteOrder
            break;
        }
        // add ID size
        targetDataByteOrder += 1;
        targetDataByteOrder += packetProperty.getDataSize(requestDataList.at(i));
    }
    // data is not existed.
    if(isPacketExisted == false)
    {
        return sensorPacketProperty::noExistedValue;
    }
    // extract target data.
    //if datasize is 1.

    if(packetProperty.getDataSize(id) == 1)
    {
        return packetData.getArrivedData().at(targetDataByteOrder);
    }

    else if(packetProperty.getDataSize(id) == 2)
    {
        int joinedData = 0;
        joinedData |= ((packetData.getArrivedData().at(targetDataByteOrder)) & 0xFF)  << 8;
        joinedData |= ((packetData.getArrivedData().at(targetDataByteOrder + 1)) & 0xFF);
        return joinedData;

    }

    return sensorPacketProperty::noExistedValue;
}

void roombaSerial::setRoombaSpeed(short reqLeftSpped,short reqRightSpped)
{
    leftSpeed = reqLeftSpped;
    rightSpeed = reqRightSpped;
    //qDebug("roombaSerial::setRoombaSpeed(reqLeftSpped: %d, reqRightSpped: %d)", reqLeftSpped, reqRightSpped);
}

void roombaSerial::writeRoombaSpeed(short reqLeftSpped,short reqRightSpped)
{
    QByteArray data;
    const int maxSpeed = 500;
    const int minSpeed = -500;
    //check max min speed for leftSpeed.
    if(reqLeftSpped < minSpeed)
    {
        reqLeftSpped = minSpeed;
    }
    else if(reqLeftSpped > maxSpeed)
    {
        reqLeftSpped = maxSpeed;
    }

    //check max min speed for rightSpeed.
    if(reqRightSpped < minSpeed)
    {
        reqRightSpped = minSpeed;
    }
    else if(reqRightSpped > maxSpeed)
    {
        reqRightSpped = maxSpeed;
    }

    //set Speed to roomba
    // Opecode: 145 -> Drive Direct
    data.append(145);
    //Right velocity high byte
    data.append((reqRightSpped >> 8) & 0xFF);
    //Right velocity Low byte
    data.append(reqRightSpped & 0xFF);
    //Left velocity high byte
    data.append((reqLeftSpped >> 8) & 0xFF);
    //Left velocity Low byte
    data.append(reqLeftSpped & 0xFF);

    writeData(data);
}

void roombaSerial::setSafeMode()
{
    QByteArray data;
    // Opecode: 131 -> Safe
    data.append(131);

    writeData(data);
}

bool roombaSerial::isValidParity(QByteArray data)
{
    bool isValid = false;

    int sum = 0;
    for(int i = 0; i < data.length(); i++)
    {
        //qDebug("data[%d]: %d", i, data.at(i));
        sum = sum + (int)(data.at(i));
    }

    //check parity
    if(0 == (sum & 0xFF))
    {
        isValid = true;
    }
    else
    {
        qDebug("dataReceived failed: %u Byte, sum is %u.", data.length(), (sum & 0xFF));
    }
    return isValid;
}
