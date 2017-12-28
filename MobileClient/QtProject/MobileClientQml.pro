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

QT += qml quick quickcontrols2 widgets positioning multimedia

CONFIG   += console debug

# Hight dpi support off
QT_AUTO_SCREEN_SCALE_FACTOR=1

# Android specific
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# iOS specific
ios {
    QMAKE_INFO_PLIST = $$PWD/ios/Info.plist

    ios_icon.files = $$files($$PWD/ios/icons/AppIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon

    QMAKE_IOS_DEPLOYMENT_TARGET=8.0
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

SOURCES += src/cpp/Main/main.cpp \
    src/cpp/Main/Application.cpp \
    src/cpp/WebApi/Picture.cpp \
    src/cpp/WebApi/QueryItem.cpp \
    src/cpp/WebApi/Spot.cpp \
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
    src/cpp/Settings/Settings.cpp \
    src/cpp/WebApi/SpotRepository.cpp \
    src/cpp/WebApi/PictureRepository.cpp \
    src/cpp/WebApi/PicturesModel.cpp \
    src/cpp/WebApi/SpotsModel.cpp \
    src/cpp/External/SimpleCrypt.cpp \
    src/cpp/HelperClasses/UltraQmlAccessManagerFactory.cpp \
    src/cpp/HelperClasses/UltraNetworkAccessManager.cpp \
    src/cpp/WebApi/CurrentUser.cpp \
    src/cpp/WebApi/User.cpp \
    src/cpp/WebApi/UserRepository.cpp

HEADERS += \
    src/cpp/Main/Application.h \
    src/cpp/WebApi/Picture.h \
    src/cpp/WebApi/QueryItem.h \
    src/cpp/WebApi/Spot.h \
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
    src/cpp/Settings/Settings.h \
    src/cpp/Settings/Secret.h \
    src/cpp/WebApi/SpotRepository.h \
    src/cpp/WebApi/PictureRepository.h \
    src/cpp/WebApi/PicturesModel.h \
    src/cpp/WebApi/SpotsModel.h \
    src/cpp/External/SimpleCrypt.h \
    src/cpp/HelperClasses/UltraQmlAccessManagerFactory.h \
    src/cpp/HelperClasses/UltraNetworkAccessManager.h \
    src/cpp/WebApi/CurrentUser.h \
    src/cpp/WebApi/User.h \
    src/cpp/WebApi/UserRepository.h \
    src/cpp/Main/main.h

RESOURCES += src/qml.qrc

DISTFILES += \
    src/qml/main.qml \
    src/qml/Navigator.qml \
    src/qml/Panel_Splash.qml \
    src/qml/Panel_Home.qml \
    src/qml/Panel_NearbySpots.qml \
    src/qml/Panel_Upload.qml \
    src/qml/Panel_User.qml \
    src/qml/TabWidgetBottom_Main.qml \
    src/qml/declarative-camera/CameraListButton.qml \
    src/qml/declarative-camera/CameraListPopup.qml \
    src/qml/declarative-camera/CameraPropertyButton.qml \
    src/qml/declarative-camera/CameraPropertyPopup.qml \
    src/qml/declarative-camera/CameraUi.qml \
    src/qml/declarative-camera/FocusButton.qml \
    src/qml/declarative-camera/PhotoCaptureControls.qml \
    src/qml/declarative-camera/PhotoPreview.qml \
    src/qml/declarative-camera/Popup.qml \
    src/qml/declarative-camera/VideoPreview.qml \
    src/qml/pages-picture/Page_PicturesList.qml \
    src/qml/pages-picture/Page_Picture.qml \
    src/qml/pages-picture/Page_Likes.qml \
    src/qml/pages-picture/PictureViewDelegate.qml \
    src/qml/pages-spot/Page_Spot.qml \
    src/qml/pages-spot/Page_SpotsList.qml \
    src/qml/pages-spot/Page_SpotsMap.qml \
    src/qml/pages-upload/Upload_AddNewSpot.qml \
    src/qml/pages-user/Page_Login.qml \
    src/qml/pages-user/Page_Register.qml \
    src/qml/pages-user/Page_Settings.qml \
    src/qml/pages-user/Page_Settings_ReportProblem.qml \
    src/qml/pages-user/Page_SignIn.qml \
    src/qml/pages-user/Page_User.qml \
    src/qml/views/SpotOverviewDelegate.qml \
    src/qml/widgets/CachedPicture.qml \
    src/qml/widgets/Link.qml \
    src/qml/widgets/TabButton.qml \
    src/qml/widgets/TabWidget.qml \
    src/qml/widgets/TabWidgetDynamicTab.qml \
    src/qml/component/Component_Dialog.qml \
    src/qml/component/Component_CurrentLocationIndicator.qml \
    src/qml/component/Component_ListSelectionDialog.qml \
    src/qml/component/Component_PicturesGrid.qml \
    src/qml/component/Component_TagSwitch.qml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
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
    ../../Documentation/My_Skate_Map_stralciata.kml \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Controller/ApplicationHelper.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Controller/CommandSet.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Controller/NewsController.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Controller/Response.php \
    ../../WebSite/Symfony/src/Instaspots/SpotsBundle/Controller/ParameterSet.php \
    ../../WebSite/Test/webserviceTest.sh \
    src/qml/pages-upload/Upload_SourceSelection.qml \
    src/qml/pages-upload/Upload_AddToExistingSpot.qml \
    src/qml/pages-upload/Upload_NearbySpotSelection.qml \
    src/qml/pages-upload/Upload_LocationCheck.qml \
    src/qml/pages-upload/Upload_CropPicture.qml \
    ../../WebSite/Symfony/app/AppKernel.php \
    ../../WebSite/Symfony/app/config/config.yml \
    ../../WebSite/Symfony/web/app_dev.php

