#include "roombaoicontroller.h"
#include "mainwindow.h"
#include "roombaserial.h"
#include "sensorpacketproperty.h"
#include "dataprocessor.h"
#include "roombaodometry.h"
#include "roombastatemachine.h"
#include "roombacoordinateframe.h"

#include <QtCore/QDebug>


roombaOiController::roombaOiController()
{
    //set Request data.
    QByteArray reqList;

    reqList.append(OpenInterfaceMode);
    reqList.append(EncoderCountsLeft);
    reqList.append(EncoderCountsRight);
    reqList.append(VelocityLeft);
    reqList.append(VelocityRight);
    reqList.append(LightBumpLeft);
    reqList.append(LightBumpFrontLeft);
    reqList.append(LightBumpCenterLeft);
    reqList.append(LightBumpCenterRight);
    reqList.append(LightBumpFrontRight);
    reqList.append(LightBumpRight);
    reqList.append(BumpsWheeldrops);

    /********************************************/
    /*      initialize Serial interface.        */
    /********************************************/
    roomba = new roombaSerial();
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();

    // use first device.
    QSerialPortInfo info = infoList.at(0);
    // Example use QSerialPort
    roomba = new roombaSerial();
    roomba->setPort(info);

    roomba->setRequestDataList(reqList);
    roomba->startStreamData();

    /********************************************/
    /*      initialize dataProcessor            */
    /********************************************/
    dp = new dataProcessor();
    dp->setRoombaSerial(roomba);
    connect((QObject*)roomba, SIGNAL(streamDataReady()), dp, SLOT(dataReceived()));

    /********************************************/
    /*      initialize MainWindows              */
    /********************************************/
    w = new MainWindow();
    w->setRoombaSerial(roomba);
    connect((QObject*)roomba, SIGNAL(streamDataReady()), w, SLOT(dataUpdated()));
    w->show();

    /********************************************/
    /*      initialize Odometory Module         */
    /********************************************/
    roombaOdometry = new RoombaOdometry();
    roombaOdometry->setRoombaSerial(roomba);
    connect((QObject*)roomba, SIGNAL(streamDataReady()), roombaOdometry, SLOT(roombaSerialDataUpdated()));
    connect((QObject*)roombaOdometry, SIGNAL(odometryDataChanged()), w, SLOT(odometyDataUpdated()));
    w->setRoombaOdometry(roombaOdometry);

    /********************************************/
    /*   initialize RoombaStateMachine Module   */
    /********************************************/
    myRoombaStateMachine = new RoombaStateMachine();
    myRoombaStateMachine->setRoombaOdometory(roombaOdometry);
    w->getRoombaCoordinateFrame()->setRoombaStateMachine(myRoombaStateMachine);
    connect(roombaOdometry, SIGNAL(odometryDataChanged()), (QObject*)myRoombaStateMachine, SLOT(process()));
    myRoombaStateMachine->setroombaSerial(roomba);
}

roombaOiController::~roombaOiController()
{
    qDebug("~roombaOiController()");

    /********************************************/
    /*      desctuct Odometory Module           */
    /********************************************/
    disconnect((QObject*)roombaOdometry, SIGNAL(odometryDataChanged()), w, SLOT(odometyDataUpdated()));
    disconnect((QObject*)roomba, SIGNAL(streamDataReady()), roombaOdometry, SLOT(roombaSerialDataUpdated()));

    roomba->stopStreamData();
    delete w;

}

