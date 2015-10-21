/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      27.03.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/views"

Item{
    id: page_Settings

    // Navigation properties
    property string navigation_Title:                 qsTr("Settings")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false

    // Signals -----------------------------
    Connections {
        target: hc_LocationManager
        onUpdate:
        {
            text_CurrentPosition.text = qsTr("Lat: %1 Lon: %2").arg(hc_LocationManager.latitude()).arg(hc_LocationManager.longitude())
        }
    }

    // Gui ---------------------------------
    Column{
        id: column
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        width: parent.width / 1.1
        spacing: 5

        // Application version
        Item{
            height: text_ApplicationVersion.height
            width: parent.width
            Text{
                id: text_ApplicationVersion
                text: qsTr("Application version")
            }

            Text{
                anchors.right: parent.right
                text: hc_Application.version()
            }
        }

        // Build timestamp
        Item{
            height: text_BuilTimestamp.height
            width: parent.width
            Text{
                id: text_BuilTimestamp
                text: qsTr("Built on")
            }

            Text{
                anchors.right: parent.right
                text: hc_Application.buildTimestamp()
            }
        }

        // Platform
        Item{
            height: text_ApplicationVersion.height
            width: parent.width
            Text{
                text: qsTr("Plateform")
            }

            Text{
                anchors.right: parent.right
                text: hc_PlateformDetail.name()
            }
        }

        // Development mode
        Item{
            height: text_DevelopmentMode.height
            width: parent.width
            Text{
                id: text_DevelopmentMode
                text: qsTr("Development mode")
            }

            Switch{
                anchors.right: parent.right
                checked: hc_Application.developmentMode()
                onCheckedChanged: hc_Application.setDevelopmentMode(checked)
            }
        }

        Rectangle{
            height: 2
            width: parent.width
            color: "#7fb5be"
        }

        // GPS
        Item{
            height: text_CurrentPosition.height
            width: parent.width
            Text{
                text: qsTr("Current position")
            }

            Text{
                id: text_CurrentPosition
                anchors.right: parent.right
                text: qsTr("Lat: %1 Lon: %2").arg(hc_LocationManager.latitude()).arg(hc_LocationManager.longitude())
            }
        }

        ComboBox {
            currentIndex: 0
            model: ListModel {
                id: cbItems
                ListElement { text: "GPS Location";                       latitude: 0;         longitude: 0        }
                ListElement { text: "Custom Location";                    latitude: 0;         longitude: 0        }
                ListElement { text: "Airolo - Camino";                    latitude: 46.527863; longitude: 8.599377 }
                ListElement { text: "Airolo - Diga della Sella";          latitude: 46.558152; longitude: 8.595343 }
                ListElement { text: "Airolo - Tre scalini banca";         latitude: 46.528897; longitude: 8.611414 }
                ListElement { text: "Biasca - Manual pad";                latitude: 46.360764; longitude: 8.969275 }
                ListElement { text: "Biasca - SPAI";                      latitude: 46.349831; longitude: 8.969629 }
                ListElement { text: "Näfels";                             latitude: 47.107661; longitude: 9.064581 }
            }
            width: 200
            onCurrentIndexChanged:
            {
                if(cbItems.get(currentIndex).text === "GPS Location")
                {
                    // Request location update
                    hc_LocationManager.requestLocation();
                    return;
                }

                if(cbItems.get(currentIndex).text === "Custom Location")
                {
                    dialog_CustomLocation.visible = true;
                    return;
                }

                hc_LocationManager.setFakePosition(cbItems.get(currentIndex).latitude,
                                                   cbItems.get(currentIndex).longitude)
            }
        }
    } // Column
    Button {
        id: button_Logout
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 5
        width: parent.width / 1.1
        text: qsTr("Logout")
        onClicked:
        {
            wa_User.logout();
            stackView.pop();
            tabWidget_Main.setCurrentItem(panel_Home);
        }
    }


    // Dialogs
    Dialog {
        id: dialog_CustomLocation
        visible: false
        title: "Custom Location"
        standardButtons: StandardButton.Ok | StandardButton.Cancel

        onAccepted: hc_LocationManager.setFakePosition(textEdit_Latitude.text,
                                                       textEdit_Longitude.text)

        Column {
            anchors.leftMargin: 5
            width: parent.width / 1.1
            spacing: 5
            Text {
                width: parent.width
                text: "Latitude:"
            }
            TextEdit {
                id: textEdit_Latitude
                width: parent.width

            }

            Text {
                width: parent.width
                text: "Longitude:"
            }
            TextEdit {
                id: textEdit_Longitude
                width: parent.width

            }
        }
    }
}

