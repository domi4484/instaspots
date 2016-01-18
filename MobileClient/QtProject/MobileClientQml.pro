#-------------------------------------------------
#
# Project Lowerspot
#
# (C) 2014-2016 Damiano Lombardi
#
#-------------------------------------------------

TEMPLATE = app

TARGET = Lowerspot

MOC_DIR            = build
RCC_DIR            = build
UI_DIR             = build
!win32:OBJECTS_DIR = build

DESTDIR = bin

QT += qml quick widgets positioning multimedia

CONFIG   += console debug

# Android specific
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# iOS specific
ios {
    QMAKE_INFO_PLIST = $$PWD/ios/Info.plist

    ios_icon.files = $$files($$PWD/ios/icons/AppIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
}

SOURCES += src/cpp/main.cpp \
    src/cpp/WebApi/Picture.cpp \
    src/cpp/WebApi/QueryItem.cpp \
    src/cpp/WebApi/Spot.cpp \
    src/cpp/WebApi/User.cpp \
    src/cpp/WebApi/WebApi.cpp \
    src/cpp/WebApi/WebApiCommand.cpp \
    src/cpp/WebApi/WebApiError.cpp \
    src/cpp/HelperClasses/Logger.cpp \
    src/cpp/HelperClasses/StringHelper.cpp \
    src/cpp/WebApi/PictureUploader.cpp \
    src/cpp/HelperClasses/PlateformDetail.cpp \
    src/cpp/HelperClasses/LocationManager.cpp \
    src/cpp/HelperClasses/PictureCacher.cpp \
    src/cpp/HelperClasses/ApplicationHelper.cpp \
    src/cpp/Settings.cpp \
    src/cpp/WebApi/SpotRepository.cpp \
    src/cpp/WebApi/PictureRepository.cpp \
    src/cpp/WebApi/PicturesModel.cpp \
    src/cpp/WebApi/SpotsModel.cpp

RESOURCES += src/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/cpp/WebApi/Picture.h \
    src/cpp/WebApi/QueryItem.h \
    src/cpp/WebApi/Spot.h \
    src/cpp/WebApi/User.h \
    src/cpp/WebApi/WebApi.h \
    src/cpp/WebApi/WebApiCommand.h \
    src/cpp/WebApi/WebApiError.h \
    src/cpp/HelperClasses/Logger.h \
    src/cpp/HelperClasses/StringHelper.h \
    src/cpp/WebApi/PictureUploader.h \
    src/cpp/HelperClasses/PlateformDetail.h \
    src/cpp/HelperClasses/LocationManager.h \
    src/cpp/HelperClasses/PictureCacher.h \
    src/cpp/HelperClasses/ApplicationHelper.h \
    src/cpp/Settings.h \
    src/cpp/WebApi/SpotRepository.h \
    src/cpp/WebApi/PictureRepository.h \
    src/cpp/WebApi/PicturesModel.h \
    src/cpp/WebApi/SpotsModel.h

DISTFILES += \
    ../../WebSite/Symfony/app/config/config_dev.yml \
    ../../WebSite/Symfony/app/config/parameters.yml \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Controller/WebserviceController.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Controller/AdvertController.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Entity/PictureRepository.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Entity/Picture.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Entity/Spot.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Entity/SpotRepository.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Resources/views/Advert/download.html.twig \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Resources/views/Advert/index.html.twig \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Resources/views/Advert/menu.html.twig \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Resources/config/routing.yml \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Resources/config/services.yml \
    ../../WebSite/Symfony/src/Instaspots/UserBundle/Entity/User.php \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    ../../Documentation/My_Skate_Map_stralciata.kml

