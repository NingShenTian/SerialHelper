#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T14:26:19
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialHelper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    frminput.cpp \
    frmnum.cpp

HEADERS  += mainwindow.h \
    frminput.h \
    frmnum.h

FORMS    += mainwindow.ui \
    frminput.ui \
    frmnum.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    res.qrc

