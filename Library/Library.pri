
message($$PWD)

INCLUDEPATH += \
  $$PWD/src

SOURCES += \
  $$PWD/src/HelperClasses/Exception.cpp \
  $$PWD/src/HelperClasses/Logger.cpp \
  $$PWD/src/Command/Command.cpp \
  $$PWD/src/Command/CommandReceiver.cpp \
  $$PWD/src/Command/CommandSender.cpp \
  $$PWD/src/Command/CommandSet.cpp \
  $$PWD/src/CommandSet/ApplicationCommandSet.cpp \
  $$PWD/src/CommandSet/SpotCommandSet.cpp \
  $$PWD/src/TcpIp/TcpIpServer.cpp \
  $$PWD/src/TcpIp/TcpIpServerConnection.cpp \
  $$PWD/src/TcpIp/TcpIpClientConnection.cpp

HEADERS += \
  $$PWD/src/HelperClasses/Exception.h \
  $$PWD/src/HelperClasses/Logger.h \
  $$PWD/src/Command/Command.h \
  $$PWD/src/Command/CommandReceiver.h \
  $$PWD/src/Command/CommandSender.h \
  $$PWD/src/Command/CommandSet.h \
  $$PWD/src/CommandSet/ApplicationCommandSet.h \
  $$PWD/src/CommandSet/SpotCommandSet.h \
  $$PWD/src/TcpIp/TcpIpServer.h \
  $$PWD/src/TcpIp/TcpIpServerConnection.h \
  $$PWD/src/TcpIp/TcpIpClientConnection.h
