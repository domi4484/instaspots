/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      13.12.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtPositioning 5.2

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"


ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 340
    height: 500

    MenuBar {
        id: menubar
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Logout")
                onTriggered: wa_User.logout();
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Component.onCompleted:{
        if(hc_PlateformDetail.isMobile() === false)
        {
            applicationWindow.menuBar = menubar;
        }

        // Try to login
        wa_User.login();

        // Check newer version
        hc_Application.checkCurrentClientVersion();
    }

    // Gui ---------------------------------
    TabWidgetBottom {
        id: tabWidget_Main
        anchors.fill: parent

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
            property string tabWidget_ButtonIconSource: "qrc:/icon/icon/im-user-offline.png"
        }
    }


    // Positioning
    PositionSource {
        id: positionSource
        onPositionChanged:
        {
            if(positionSource.valid === false)
            {
                hc_Logger.slot_error("PositionSource invalid source");
                return;
            }

            if(positionSource.sourceError !== PositionSource.NoError)
            {
                hc_Logger.slot_error("PositionSource source error: " + positionSource.sourceError);
                return;
            }

            if(   position.latitudeValid  === false
               || position.longitudeValid === false)
            {
                hc_Logger.slot_error("PositionSource invalid position");
                return;
            }

            hc_LocationManager.setFakePosition(position.coordinate.latitude,
                                               position.coordinate.longitude)
        }
    }

    Connections{
        target: hc_LocationManager
        onSignal_RequestLocation: positionSource.update()
    }

    Connections{
        target: hc_Application
        onSignal_NewClientVersionAvailable:
        {
            messageDialog_NewClientVersionAvailable.open();
        }
    }

    // Message dialogs ---------------------
    MessageDialog{
        id: messageDialog_NewClientVersionAvailable
        title: qsTr('New version out!')
        text: qsTr('There is a new Lowerspot app version available for download! Get it on lowerclassclothing.com!')

        onAccepted:
        {
            hc_Application.newerClientVersionAvailableGotIt();
        }
    }
}
