/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      22.02.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"


// Gui -------------------------------------
TabWidgetBottom {
    id: tabWidget_Main
    width : parent.width
    height: parent.height

    Panel_Home {
        id: panel_Home
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/folder-home.png"
    }

    Panel_NearbySpots {
        id: panel_NearbySpots
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/find-location.png"
    }

    Panel_Upload {
        id: panel_Upload
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/camera-photo.png"
    }

    Panel_User {
        id: panel_User
        anchors.fill: parent
        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/avatar-default.png"
    }
}

