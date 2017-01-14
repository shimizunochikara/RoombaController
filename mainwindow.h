#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QDebug>

class roombaSerial;
class RoombaOdometry;
class RoombaCoordinateFrame;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    void setRoombaSerial(roombaSerial *rs);
    void setRoombaOdometry(RoombaOdometry* ro);
    RoombaCoordinateFrame* getRoombaCoordinateFrame(){return frame;};


private:
    static const int batteryCapacity = 2696;
    roombaSerial *myRoombaSerial;
    int reqRightVerocity, reqLeftVerocity, reqSpeed;
    int oiMode;
    int leftRoteryEncoderCount, rightRoteryEncoderCount;

    //below parameter is roomba auto control

    RoombaOdometry *myRoombaOdometry;
    RoombaCoordinateFrame *frame;


public slots:
    void dataUpdated();
    void setReqSpeed(int left, int right);

};

#endif // MAINWINDOW_H
