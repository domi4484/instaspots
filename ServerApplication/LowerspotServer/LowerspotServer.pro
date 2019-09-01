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

# Common files
include($$PWD/../../Library/Library.pri)


SOURCES += \
  src/Main/main.cpp \
  src/Main/Application.cpp \
  src/CommandReceiver/ApplicationCommandReceiver.cpp \
  src/Settings/Settings.cpp

HEADERS += \
  src/Main/Application.h \
  src/CommandReceiver/ApplicationCommandReceiver.h \
  src/Settings/Settings.h

