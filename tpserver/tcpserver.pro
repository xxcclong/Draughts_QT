#-------------------------------------------------
#
# Project created by QtCreator 2015-08-29T12:57:50
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += core network
QT+=xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpserver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
