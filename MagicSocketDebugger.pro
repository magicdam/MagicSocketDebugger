#-------------------------------------------------
#
# Project created by QtCreator 2018-11-14T22:13:59
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagicSocketDebugger
TEMPLATE = app

ICON = resource/icon.icns
RC_ICONS = resource/icon.png

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    connection.cpp \
    tcp/tcpclient.cpp \
    tcp/tcpserver.cpp \
    tcp/tcpserverconnection.cpp \
    websocket/wsclient.cpp \
    websocket/wsserver.cpp \
    websocket/wsserverconnection.cpp

HEADERS += \
        mainwindow.h \
    connection.h \
    tcp/tcpclient.h \
    tcp/tcpserver.h \
    tcp/tcpserverconnection.h \
    websocket/wsclient.h \
    websocket/wsserver.h \
    websocket/wsserverconnection.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
