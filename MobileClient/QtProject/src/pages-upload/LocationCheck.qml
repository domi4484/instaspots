/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
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
import QtQuick.Dialogs 1.2
import QtPositioning 5.2

// Project qml imports ---------------------
import "qrc:/"

Item{
    id: locationCheck
    visible: false

    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Location")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Signals -----------------------------

    signal locationAccepted()

    onVisibleChanged: {
        if(visible == false)
            return;

        image_GoogleMapImage.source = hc_LocationManager.googleMapLocationPicture(image_GoogleMapImage.width,
                                                                                  image_GoogleMapImage.height);
    }

    Connections {
        target: hc_LocationManager
        onUpdate:
        {
            if(visible == false)
                return;

            if(timeout)
                return;

            image_GoogleMapImage.source = hc_LocationManager.googleMapLocationPicture(image_GoogleMapImage.width,
                                                                                      image_GoogleMapImage.height);
        }
    }


    // Gui ---------------------------------

    Image{
        id: image_GoogleMapImage
        width: parent.width
        height: parent.height
        anchors.top: parent.top
    }

    Button{
        id: button_UpdateLocation
        text: qsTr("Update location")

        width: parent.width / 1.1
        anchors.top: parent.top
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
            if(hc_LocationManager.isValid() === false)
            {
                messageDialog_LocationError.visible = true;
            }

            wa_PictureUploader.setPosition(hc_LocationManager.latitude(),
                                           hc_LocationManager.longitude());
            locationAccepted();
        }
    }


    // Message boxes -----------------------

    MessageDialog{
        id: messageDialog_LocationError
        title: qsTr('Location error')
        text: qsTr('Can\'t determine your current location in a precise way.')
    }
}
