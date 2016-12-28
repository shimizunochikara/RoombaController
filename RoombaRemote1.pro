#-------------------------------------------------
#
# Project created by QtCreator 2016-09-15T19:16:11
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RoombaRemote1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
   roombaserial.cpp \
    sensorpacketproperty.cpp \
    roombaoiController.cpp \
    dataprocessor.cpp \
    goandbacksequence.cpp \
    roombaodometry.cpp \
    roombacoordinateframe.cpp \
    roombastatemachine.cpp

HEADERS  += mainwindow.h \
    roombaserial.h \
    sensorpacketproperty.h \
    roombaoicontroller.h \
    dataprocessor.h \
    goandbacksequence.h \
    roombaodometry.h \
    roombacoordinateframe.h \
    roombastatemachine.h

FORMS    += mainwindow.ui
