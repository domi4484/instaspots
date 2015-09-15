
// Qt imports ------------------------------
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import QtPositioning 5.2

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 340
    height: 480

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

    PositionSource {
        id: positionSource
        onPositionChanged: hc_LocationManager.setFakePosition(position.coordinate.latitude,
                                                              position.coordinate.longitude)
    }

    Component.onCompleted:{
        if(hc_PlateformDetail.isMobile() === false)
        {
            applicationWindow.menuBar = menubar;
        }

        // Try to login
        wa_User.login();

        pageLoader.source = "Panel_Main.qml";
    }

    Loader {
        id: pageLoader
        anchors.fill: parent
    }

    Connections {
        target: wa_User
        onSignal_LoginSuccessfull:
        {
            // Login successfull
            if(success)
            {
                pageLoader.source = "Panel_Main.qml";
                return;
            }

            if(pageLoader.source != Qt.resolvedUrl("Panel_Login.qml"))
            {
                pageLoader.source = "Panel_Login.qml";
            }
        }

        onSignal_Logout:
        {
            pageLoader.source = "Panel_Login.qml";
        }
    }

    Connections{
        target: hc_LocationManager
        onSignal_RequestLocation: positionSource.update()
    }
}
