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
    frame = new RoombaCoordinateFrame(ui->page_2);
    connect(ui->removeLastPointButton, SIGNAL(pressed()), frame, SLOT(removePreviousPoint()));
    connect(ui->removeAllPointsButton, SIGNAL(pressed()), frame, SLOT(removeAllPoints()));

    connect(ui->startMovingButton, SIGNAL(pressed()), frame, SLOT(startButtonPressed()));
    connect(ui->stopMovingButton, SIGNAL(pressed()), frame, SLOT(stopButtonPressed()));

    reqRightVerocity = 0;
    reqLeftVerocity = 0;
    reqSpeed = 100;
    //OI mode -> Off: 0, Passive: 1, Safe: 2, Full: 3
    oiMode = 0;

    odometyDataUpdatedCount = 0;
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
    //update bumper data
    ui->value_lt_Left->setText(QString::number(myRoombaSerial->fetchPacket(LightBumpLeft)));
    ui->value_lt_frontLeft->setText(QString::number(myRoombaSerial->fetchPacket(LightBumpFrontLeft)));
    ui->value_lt_centerleft->setText(QString::number(myRoombaSerial->fetchPacket(LightBumpCenterLeft)));
    ui->value_lt_centerright->setText(QString::number(myRoombaSerial->fetchPacket(LightBumpCenterRight)));
    ui->value_lt_frontright->setText(QString::number(myRoombaSerial->fetchPacket(LightBumpFrontRight)));
    ui->value_lt_right->setText(QString::number(myRoombaSerial->fetchPacket(LightBumpRight)));

    //update rotery encoder data
    ui->value_re_left->setText(QString::number(myRoombaSerial->fetchPacket(EncoderCountsLeft)));
    ui->value_re_right->setText(QString::number(myRoombaSerial->fetchPacket(EncoderCountsRight)));
    leftRoteryEncoderCount = myRoombaSerial->fetchPacket(EncoderCountsLeft);
    rightRoteryEncoderCount = myRoombaSerial->fetchPacket(EncoderCountsRight);

    //update bumber data
    ui->value_bumper_left->setText(QString::number((myRoombaSerial->fetchPacket(BumpsWheeldrops) & 0x02) >> 1));
    ui->value_bumper_right->setText(QString::number(myRoombaSerial->fetchPacket(BumpsWheeldrops) & 0x01));

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

void MainWindow::odometyDataUpdated()
{
    if(odometyDataUpdatedCount % 10 == 0)
    {
        ui->value_velocity_total->setText(QString::number(myRoombaOdometry->getVerocity()) + "mm/s");
        ui->value_velocity_left->setText(QString::number(myRoombaOdometry->getLeftVelocity()) + "mm/s");
        ui->value_velocity_right->setText(QString::number(myRoombaOdometry->getRightVerocity()) + "mm/s");
        ui->value_velocity_angular->setText(QString::number(myRoombaOdometry->getAngularVelocity()) + " Radius/s");
        ui->coordinate_x->setText(QString::number(myRoombaOdometry->getXCoordinate()) + "mm");
        ui->coordinate_y->setText(QString::number(myRoombaOdometry->getYCoordinate()) + "mm");
        ui->coordinate_angular->setText(QString::number(myRoombaOdometry->getAngularCoordinate()) + " Radius");

        //second windows update
        ui->coordinate_x_2->setText(QString::number(myRoombaOdometry->getXCoordinate()) + "mm");
        ui->coordinate_y_2->setText(QString::number(myRoombaOdometry->getYCoordinate()) + "mm");
        ui->coordinate_angular_2->setText(QString::number(myRoombaOdometry->getAngularCoordinate()) + " Radius");
    }
    odometyDataUpdatedCount ++;
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        reqRightVerocity = 1 * reqSpeed;
        reqLeftVerocity = 1 * reqSpeed;
        break;
    case Qt::Key_Down:
        reqRightVerocity = -1 * reqSpeed;
        reqLeftVerocity = -1 * reqSpeed;
        break;
    case Qt::Key_Left:
        reqRightVerocity = 1 * reqSpeed;
        reqLeftVerocity = -1 * reqSpeed;
        break;
    case Qt::Key_Right:
        reqRightVerocity = -1 * reqSpeed;
        reqLeftVerocity = 1 * reqSpeed;
        break;
    default:
        break;
    }
}


void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        reqRightVerocity = 0;
        reqLeftVerocity = 0;
        break;
    case Qt::Key_Down:
        reqRightVerocity = 0;
        reqLeftVerocity = 0;
        break;
    case Qt::Key_Left:
        reqRightVerocity = 0;
        reqLeftVerocity = 0;
        break;
    case Qt::Key_Right:
        reqRightVerocity = 0;
        reqLeftVerocity = 0;
        break;
    default:
        break;
    }
}

void MainWindow::setReqSpeed(int left, int right)
{
    reqLeftVerocity = left;
    reqRightVerocity = right;
}
