#ifndef GOANDBACKSEQUENCE_H
#define GOANDBACKSEQUENCE_H

#include <QThread.h>

class roombaSerial;

class goAndBackSequence : public QThread
{
    Q_OBJECT
public:
    goAndBackSequence();
    void setRoombaSerial(roombaSerial* rs);
    void setRoteryEncoderCount(int left, int right);
    void setBumper(int left, int right);
    void run();

signals:

public slots:

private:
    roombaSerial *myRoombaSerial;
    bool isRoombaOnAutoSequence;
    int previousLeftRoteryEncoderCount, previousRightRoteryEncoderCount;
    int accumulatedLeftRoteryEncoderCount,accumulatedRightRoteryEncoderCount;
    int startLeftRoteryEncoderCount,startRightRoteryEncoderCount;
    static const int  TurnRoteryEncoderCount = 900;

    int leftBumper, rightBumper;
signals:
    void updateVelocity(int left, int right);
};

#endif // GOANDBACKSEQUENCE_H
