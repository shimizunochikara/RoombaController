#ifndef ROOMBACOORDINATEFRAME_H
#define ROOMBACOORDINATEFRAME_H

#include <QtCore>
#include <QFrame>
#include <QVector>
#include <QPainter>
#include <QFocusEvent>

class RoombaStateMachine;

class RoombaCoordinateFrame : public QFrame
{
    Q_OBJECT
public:
    RoombaCoordinateFrame(QWidget *qw);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void drawMousePointer(QPainter &painter);
    void drawPoint(QPoint point, QString str, QPainter &painter);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    QVector<QPoint>* getMovePointVector();
    int changeReferencePointToCenter_X(int point);
    int changeReferencePointToCenter_Y(int point);
    int changeReferencePointToUpperLeft_X(int point);
    int changeReferencePointToUpperLeft_Y(int point);
    void setRoombaStateMachine(RoombaStateMachine* rsm);


private:
    QVector<QPoint> movePoint;
    QPoint mousePoint;
    bool isFocusIn;
    RoombaStateMachine* myRoombaStateMachine;
signals:

public slots:
    void removePreviousPoint();
    void removeAllPoints();
    void changePreviousPointToHomebase();
    void startButtonPressed();
    void stopButtonPressed();
};

#endif // ROOMBACOORDINATEFRAME_H
