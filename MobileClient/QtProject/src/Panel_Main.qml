/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:                                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.2
import QtQuick.Controls 1.1

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/widgets"

TabWidget {
    id: tabs
    width: 640; height: 480

    Panel_Home {
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/folder-home.png"
    }

    Panel_NearbySpots {
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/find-location.png"
    }

    Panel_Upload {
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/camera-photo.png"
    }

    Panel_User {
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/im-user-offline.png"
    }
}
