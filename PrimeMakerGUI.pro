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
    dialograpidtask.cpp \
    dialogcustomtask_file.cpp \
    dialogcustomtask_fragment.cpp \
    dialogcustomtask_pair.cpp \
    dialogaboutme.cpp

HEADERS  += mainwindow.h \
    dialogsettingproperties.h \
    dialograpidtask.h \
    dialogcustomtask_file.h \
    dialogcustomtask_fragment.h \
    dialogcustomtask_pair.h \
    dialogaboutme.h \
    datadef.h

FORMS    += mainwindow.ui \
    dialogsettingproperties.ui \
    dialograpidtask.ui \
    dialogcustomtask_file.ui \
    dialogcustomtask_fragment.ui \
    dialogcustomtask_pair.ui \
    dialogaboutme.ui
