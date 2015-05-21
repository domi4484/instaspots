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

// Project imports -------------------------
import "qrc:/"
import "qrc:/views"

BasicPage{
    id: page_Settings

    // BasicPage properties ----------------
    title: qsTr("Settings")
    continueButtonVisible: false
    menuButtonVisible: false

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
                ListElement { text: "GPS Location";                 latitude: 0;         longitude: 0        }
                ListElement { text: "Custom Location";              latitude: 0;         longitude: 0        }
                ListElement { text: "Airolo - Camino";              latitude: 46.527863; longitude: 8.599377 }
                ListElement { text: "Airolo - Diga della Sella";    latitude: 46.558152; longitude: 8.595343 }
                ListElement { text: "Airolo - Tre scalini banca";   latitude: 46.528897; longitude: 8.611414 }
                ListElement { text: "Bellinzona - Piazza del sole"; latitude: 46.193301; longitude: 9.024003 }
                ListElement { text: "Bellinzona - Cervia";          latitude: 46.190579; longitude: 9.023166 }
                ListElement { text: "Biasca - Manual pad";          latitude: 46.360764; longitude: 8.969275 }
                ListElement { text: "Biasca - SPAI";                latitude: 46.349831; longitude: 8.969629 }
                ListElement { text: "Castione - Bank";              latitude: 46.221177; longitude: 9.042542 }
                ListElement { text: "Chur - Curb Bahnhof";          latitude: 46.853456; longitude: 9.527019 }
                ListElement { text: "Chur - Bank";                  latitude: 46.851196; longitude: 9.528907 }
            }
            width: 200
            onCurrentIndexChanged:
            {
                if(cbItems.get(currentIndex).text === "Custom Location")
                {
                   // TODO dialogo posizionamento
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
        onClicked: wa_User.logout()
    }
}

