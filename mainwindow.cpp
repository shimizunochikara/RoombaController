#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "roombaserial.h"
#include "roombacoordinateframe.h"

#include <QKeyEvent>
#include <QPainter>

#include "roombaodometry.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initilize QFrame
    frame = new RoombaCoordinateFrame(parent);
    connect(ui->removeLastPointButton, SIGNAL(pressed()), frame, SLOT(removePreviousPoint()));
    connect(ui->removeAllPointsButton, SIGNAL(pressed()), frame, SLOT(removeAllPoints()));

    connect(ui->startMovingButton, SIGNAL(pressed()), frame, SLOT(startButtonPressed()));
    connect(ui->stopMovingButton, SIGNAL(pressed()), frame, SLOT(stopButtonPressed()));

    reqRightVerocity = 0;
    reqLeftVerocity = 0;
    reqSpeed = 100;
    //OI mode -> Off: 0, Passive: 1, Safe: 2, Full: 3
    oiMode = 0;

}

MainWindow::~MainWindow()
{
    //disconnect(ui->autoSequenceButton, SIGNAL(pressed()), this, SLOT(autoButtonPressed()));
    disconnect(ui->removeLastPointButton, SIGNAL(pressed()), frame, SLOT(removePreviousPoint()));
    disconnect(ui->removeAllPointsButton, SIGNAL(pressed()), frame, SLOT(removeAllPoints()));

    disconnect(ui->startMovingButton, SIGNAL(pressed()), frame, SLOT(startButtonPressed()));
    disconnect(ui->stopMovingButton, SIGNAL(pressed()), frame, SLOT(stopButtonPressed()));


    delete frame;
    delete ui;
}

void MainWindow::dataUpdated()
{
    //update rotery encoder data
    leftRoteryEncoderCount = myRoombaSerial->fetchPacket(EncoderCountsLeft);
    rightRoteryEncoderCount = myRoombaSerial->fetchPacket(EncoderCountsRight);

    //update OI Mode
    oiMode =  myRoombaSerial->fetchPacket(OpenInterfaceMode) & 0xFF;

    //speed control
    if(reqRightVerocity != 0 || reqLeftVerocity != 0)
    {
        if(oiMode != 2)
        {
            myRoombaSerial->setSafeMode();
        }
    }
    //myRoombaSerial->setRoombaSpeed(reqLeftVerocity, reqRightVerocity);
}


void MainWindow::setRoombaSerial(roombaSerial *rs)
{
    myRoombaSerial = rs;
}

void MainWindow::setRoombaOdometry(RoombaOdometry* ro)
{
    myRoombaOdometry = ro;
    connect(ui->resetOdometoryButton, SIGNAL(pressed()), myRoombaOdometry, SLOT(resetOdometyData()));
}

void MainWindow::setReqSpeed(int left, int right)
{
    reqLeftVerocity = left;
    reqRightVerocity = right;
}
