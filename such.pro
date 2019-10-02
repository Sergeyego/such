#-------------------------------------------------
#
# Project created by QtCreator 2017-06-19T15:01:07
#
#-------------------------------------------------

QT       += core gui serialport serialbus

CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = such
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    cfgplot.cpp \
    widgets.cpp \
    inputdialog.cpp \
    modelchannel.cpp \
    delegate.cpp \
    regs.cpp \
    mbbtn.cpp \
    mblbl.cpp \
    modelalarm.cpp \
    modeltech.cpp

HEADERS  += mainwindow.h \
    cfgplot.h \
    widgets.h \
    inputdialog.h \
    modelchannel.h \
    delegate.h \
    regs.h \
    mbbtn.h \
    mblbl.h \
    modelalarm.h \
    modeltech.h

FORMS    += mainwindow.ui \
    cfgplot.ui \
    inputdialog.ui

LIBS += -lqwt-qt5
INCLUDEPATH += /usr/include/qwt

RESOURCES += \
    res.qrc
