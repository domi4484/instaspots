#-------------------------------------------------
#
# Project Lowerspot
#
# (C) 2014-2019 Damiano Lombardi
#
#-------------------------------------------------

TEMPLATE = app

TARGET = LowerspotServer

QT += network sql
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += \
  src \

# Common files
include($$PWD/../../Library/Library.pri)


SOURCES += \
  src/Database/DatabaseManager.cpp \
  src/Database/DatabaseTablePicture.cpp \
  src/Entity/EntityPicture.cpp \
  src/Main/main.cpp \
  src/Main/Application.cpp \
  src/CommandReceiver/ApplicationCommandReceiver.cpp \
  src/Settings/Settings.cpp

HEADERS += \
  src/Database/DatabaseManager.h \
  src/Database/DatabaseTablePicture.h \
  src/Entity/EntityPicture.h \
  src/Main/Application.h \
  src/CommandReceiver/ApplicationCommandReceiver.h \
  src/Settings/Settings.h

