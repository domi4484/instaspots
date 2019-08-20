#-------------------------------------------------
#
# Project Lowerspot
#
# (C) 2014-2019 Damiano Lombardi
#
#-------------------------------------------------

TEMPLATE = app

TARGET = LowerspotServer

QT += network
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += \
  src \
  ../../Library/src

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
  ../../Library/src/Command/CommandSender.cpp \
  src/Main/main.cpp \
  src/Main/Application.cpp \
  src/CommandReceiver/ServerApplicationCommandReceiver.cpp \
  src/Settings/Settings.cpp \
  ../../Library/src/HelperClasses/Exception.cpp \
  ../../Library/src/HelperClasses/Logger.cpp \
  ../../Library/src/Command/Command.cpp \
  ../../Library/src/Command/CommandReceiver.cpp \
  ../../Library/src/Command/CommandSet.cpp \
  ../../Library/src/CommandSet/ServerApplicationCommandSet.cpp \
  ../../Library/src/TcpIp/TcpIpServer.cpp \
  ../../Library/src/TcpIp/TcpIpServerConnection.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  ../../Library/src/Command/CommandSender.h \
  src/Main/Application.h \
  src/CommandReceiver/ServerApplicationCommandReceiver.h \
  src/Settings/Settings.h \
  ../../Library/src/HelperClasses/Exception.h \
  ../../Library/src/HelperClasses/Logger.h \
  ../../Library/src/Command/Command.h \
  ../../Library/src/Command/CommandReceiver.h \
  ../../Library/src/Command/CommandSet.h \
  ../../Library/src/CommandSet/ServerApplicationCommandSet.h \
  ../../Library/src/TcpIp/TcpIpServer.h \
  ../../Library/src/TcpIp/TcpIpServerConnection.h
