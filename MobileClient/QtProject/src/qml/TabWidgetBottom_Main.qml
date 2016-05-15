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
TabWidget {
    id: tabWidget_Main

    height: parent.height
    width:  parent.width

    tabWidget_TabsOnTheBottom : true

    TabWidgetDynamicTab {
        id: panel_Home
        anchors.fill: parent

        tabWidgetDynamicTab_ContentSourceUrl: "qrc:/qml/Panel_Home.qml"

        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/folder-home.png"
    }

    TabWidgetDynamicTab {
        id: panel_NearbySpots
        anchors.fill: parent

        tabWidgetDynamicTab_ContentSourceUrl: "qrc:/qml/Panel_NearbySpots.qml"

        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/find-location.png"
    }

    TabWidgetDynamicTab {
        id: panel_Upload
        anchors.fill: parent

        tabWidgetDynamicTab_ContentSourceUrl: "qrc:/qml/Panel_Upload.qml"

        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/camera-photo.png"
    }

    TabWidgetDynamicTab {
        id: panel_User
        anchors.fill: parent

        tabWidgetDynamicTab_ContentSourceUrl: "qrc:/qml/Panel_User.qml"

        property string tabWidget_ButtonText: ""
        property string tabWidget_ButtonIconSource: "qrc:/icon/icon/avatar-default.png"
    }
}

