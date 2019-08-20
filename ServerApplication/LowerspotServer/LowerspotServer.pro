
QT += network
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += ../Externals/lowerspot-lib

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
  src/Command/Command.cpp \
  src/Command/CommandReceiver.cpp \
  src/Command/CommandSet.cpp \
  src/CommandSet/ServerApplicationCommandReceiver.cpp \
  src/CommandSet/ServerApplicationCommandSet.cpp \
  src/HelperClasses/Logger.cpp \
        src/Main/Application.cpp \
        src/Main/main.cpp \
        src/Settings/Settings.cpp \
    src/HelperClasses/Exception.cpp \
  src/TcpIp/TcpIpServer.cpp \
  src/TcpIp/TcpIpServerConnection.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  src/Command/Command.h \
  src/Command/CommandReceiver.h \
  src/Command/CommandSet.h \
  src/CommandSet/ServerApplicationCommandReceiver.h \
  src/CommandSet/ServerApplicationCommandSet.h \
  src/HelperClasses/Logger.h \
  src/Main/Application.h \
  src/Settings/Settings.h \
    src/HelperClasses/Exception.h \
  src/TcpIp/TcpIpServer.h \
  src/TcpIp/TcpIpServerConnection.h
