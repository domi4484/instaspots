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

        // GPS
        Text{
            text: qsTr("Current position")
        }

        Text{
            id: text_CurrentPosition
            text: qsTr("Lat: %1 Lon: %2").arg(hc_LocationManager.latitude()).arg(hc_LocationManager.longitude())
        }

        ComboBox {
            currentIndex: 0
            model: ListModel {
                id: cbItems
                ListElement { text: "GPS Location"; latitude: 0;         longitude: 0 }
                ListElement { text: "Chur";         latitude: 46.847174; longitude: 9.522164 }
                ListElement { text: "Chur rail up"; latitude: 46.849522; longitude: 9.530469 }
                ListElement { text: "Bellinzona";   latitude: 46.195113; longitude: 9.026962 }
                ListElement { text: "Landquart";    latitude: 46.953120; longitude: 9.566670 }
            }
            width: 200
            onCurrentIndexChanged: hc_LocationManager.setFakePosition(cbItems.get(currentIndex).latitude,
                                                                      cbItems.get(currentIndex).longitude)
        }
    }
}

