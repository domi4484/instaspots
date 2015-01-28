
// Qt imports ------------------------------
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 340
    height: 480
    title: qsTr("InstaSpots")

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

        hc_LocationManager.requestLocation();

        if(wa_User.login() === false)
        {
            pageLoader.source = "Panel_Login.qml";
        }
        else
        {
            pageLoader.source = "Panel_Splash.qml"
        }
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
}
