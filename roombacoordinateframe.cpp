#include "roombacoordinateframe.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
#include <QHoverEvent>

#include "roombastatemachine.h"


RoombaCoordinateFrame::RoombaCoordinateFrame(QWidget *qw)
    :QFrame(qw)
    ,isFocusIn(false)
{
    setObjectName(QStringLiteral("frame"));
    setGeometry(QRect(20, 60, 600, 400));
    setFrameShape(QFrame::Box);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    //set starting point
    movePoint.append(QPoint(0, 0));
    update();
}

void RoombaCoordinateFrame::drawGrid(QPainter & painter)
{
    const int GridInterval = 50;
    double height = this->geometry().height();
    double width = this->geometry().width();

    painter.setPen("Gray");

    for(int i = 1; i < height; i++)
    {
        if(i % GridInterval == 0)
        {
            painter.drawLine(QPoint(0, i), QPoint(width, i));
        }
    }

    for(int i = 1; i < width; i++)
    {
        if(i % GridInterval == 0)
        {
            painter.drawLine(QPoint(i, 0), QPoint(i, height));
        }
    }
}

void RoombaCoordinateFrame::drawPoint(QPoint point, QString str ,QPainter &painter)
{
    painter.setPen(QColor("Red"));
    int pointX = changeReferencePointToUpperLeft_X(point.x());
    int pointY = changeReferencePointToUpperLeft_Y(point.y());
    painter.drawEllipse(pointX, pointY, 3, 3);
    painter.drawText(pointX, pointY + 3, str);
}

void RoombaCoordinateFrame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //Draw frame
    painter.drawRect(contentsRect());
    QPoint center = this->geometry().center();

    drawGrid(painter);
    drawMousePointer(painter);
    for(int i = 0; i < movePoint.count(); i++)
    {
        QString str = QString::number(i);
        str += ": (";
        str += QString::number(movePoint.at(i).x());
        str += ", ";
        str += QString::number(movePoint.at(i).y());
        str += ")";
        drawPoint(movePoint.at(i),str,painter);
        //qDebug("Point%d: x = %d, y = %d", i, movePoint.at(i).x(),movePoint.at(i).y());
    }

}


void RoombaCoordinateFrame::mouseMoveEvent(QMouseEvent *event)
{

    int dx = changeReferencePointToCenter_X(event->x());      // where lastpos is a QPoint member
    int dy = changeReferencePointToCenter_Y(event->y());

    mousePoint.setX(dx);
    mousePoint.setY(dy);

    update();
}

void RoombaCoordinateFrame::drawMousePointer(QPainter &painter)
{
    if(isFocusIn)
    {
        QString str = "(";
        str += QString::number(mousePoint.x());
        str += ", ";
        str += QString::number(mousePoint.y());
        str += ")";
        painter.setPen(QColor("gray"));
        int drawPointX = changeReferencePointToUpperLeft_X(mousePoint.x());
        int drawPointY = changeReferencePointToUpperLeft_Y(mousePoint.y());

        painter.drawText(QPoint(drawPointX,drawPointY), str);
    }
}

void RoombaCoordinateFrame::mousePressEvent(QMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton)
    {  //if you have MOVEd
        int dx = changeReferencePointToCenter_X(event->x());      // where lastpos is a QPoint member
        int dy = changeReferencePointToCenter_Y(event->y());
        movePoint.append(QPoint(dx, dy));

        update();
    }
}

void RoombaCoordinateFrame::enterEvent(QEvent *)
{
    isFocusIn = true;
    update();
}

void RoombaCoordinateFrame::leaveEvent(QEvent *)
{
    isFocusIn = false;
    update();
}

QVector<QPoint>* RoombaCoordinateFrame::getMovePointVector()
{
    return &movePoint;
}

void RoombaCoordinateFrame::removePreviousPoint()
{
    if(movePoint.count() > 1)
    {
        movePoint.removeLast();
    }
    update();
}

void RoombaCoordinateFrame::removeAllPoints()
{
    while(movePoint.count() > 1)
    {
        movePoint.removeLast();
    }
    update();
}

void RoombaCoordinateFrame::changePreviousPointToHomebase()
{
    update();
}

int RoombaCoordinateFrame::changeReferencePointToCenter_X(int point)
{
    return point - this->geometry().width() / 2;
}

int RoombaCoordinateFrame::changeReferencePointToCenter_Y(int point)
{
    return -point + this->geometry().height() / 2;
}

int RoombaCoordinateFrame::changeReferencePointToUpperLeft_X(int point)
{
    return point + this->geometry().width() / 2;
}

int RoombaCoordinateFrame::changeReferencePointToUpperLeft_Y(int point)
{
    return -point + this->geometry().height() / 2;
}

void RoombaCoordinateFrame::setRoombaStateMachine(RoombaStateMachine* rsm)
{
    myRoombaStateMachine = rsm;
}

void RoombaCoordinateFrame::startButtonPressed()
{
    myRoombaStateMachine->addTargetPoints(&movePoint);
    myRoombaStateMachine->startMoving();
}

void RoombaCoordinateFrame::stopButtonPressed()
{
    myRoombaStateMachine->resetTask();
}
