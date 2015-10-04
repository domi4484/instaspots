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
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import QtPositioning 5.2

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/widgets"


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

    MessageDialog{
        id: messageDialog_NewClientVersionAvailable
        title: qsTr('New version out!')
        text: qsTr('There is a new Lowerspot app version available for download! Get it on the Lowerspot homepage for a better experience!')

        onAccepted:
        {
            hc_Application.newerClientVersionAvailableGotIt();
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
    TabWidget {
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
        onPositionChanged: hc_LocationManager.setFakePosition(position.coordinate.latitude,
                                                              position.coordinate.longitude)
    }

    Connections{
        target: hc_LocationManager
        onSignal_RequestLocation: positionSource.update()
    }

    Connections{
        target: hc_Application
        onSignal_NewClientVersionAvailable:
        {
            messageDialog_NewClientVersionAvailable.visible = true;
        }
    }
}
