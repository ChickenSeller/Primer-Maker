#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T23:28:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PrimeMakerGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogsettingproperties.cpp \
    dialograpidtask.cpp

HEADERS  += mainwindow.h \
    dialogsettingproperties.h \
    dialograpidtask.h

FORMS    += mainwindow.ui \
    dialogsettingproperties.ui \
    dialograpidtask.ui
