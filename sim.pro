#-------------------------------------------------
#
# Project created by QtCreator 2018-05-20T09:37:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = car_sim
TEMPLATE = app


SOURCES += main.cpp\
        drawCar.cpp \
    sensor_server.cpp \
    car.cpp

HEADERS  += drawCar.h \
    sensor_server.h \
    car.h



