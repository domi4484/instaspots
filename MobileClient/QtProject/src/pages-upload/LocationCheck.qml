/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      30.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtPositioning 5.2

// Project imports -------------------------
import "qrc:/"

BasicPage{
    id: locationCheck
    visible: false

    // BasiPage properties -----------------
    title: qsTr("Location")
    continueButtonVisible: false

    // Signals -----------------------------
    signal locationAccepted()

    onVisibleChanged: {
        if(visible == false)
            return;

        textField_Latitude.text = hc_LocationManager.latitude();
        textField_Longitude.text = hc_LocationManager.longitude();
    }

    Connections {
        target: hc_LocationManager
        onUpdate:
        {
            if(timeout)
                return;

            textField_Latitude.text = hc_LocationManager.latitude();
            textField_Longitude.text = hc_LocationManager.longitude();
        }
    }

    // Gui ---------------------------------
    Item{
        id: item_Latitude

        width: parent.width / 1.2
        height: 40
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        Text{
            id: text_Latitude
            height: parent.height
            width: parent.width/2
            verticalAlignment: Text.AlignVCenter
            text: qsTr("Latitude: ")
        }

        Text{
            id: textField_Latitude
            anchors.left: text_Latitude.right
            height: text_Latitude.height
            width: text_Latitude.width
        }
    }

    Item{
        id: item_Longitude

        width: parent.width / 1.2
        height: 40
        anchors.top: item_Latitude.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        Text{
            id: text_Longitude
            height: parent.height
            width: parent.width/2
            verticalAlignment: Text.AlignVCenter
            text: qsTr("Longitude: ")
        }

        Text{
            id: textField_Longitude
            anchors.left: text_Longitude.right
            height: text_Longitude.height
            width: text_Longitude.width
        }
    }

    Button{
        id: button_UpdateLocation
        text: qsTr("Update location")

        width: parent.width / 1.1
        anchors.top: item_Longitude.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked:
        {
            // Request location update
            hc_LocationManager.requestLocation();
        }
    }

    Button{
        id: button_Continue
        text: qsTr("Continue")

        width: parent.width / 1.1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked: {
            wa_PictureUploader.setPosition(hc_LocationManager.latitude(),
                                           hc_LocationManager.longitude());
            locationAccepted();
        }
    }
} // page_Location
