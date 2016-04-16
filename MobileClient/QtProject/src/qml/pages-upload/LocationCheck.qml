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
import QtQuick          2.5
import QtQuick.Controls 1.2
import QtQuick.Dialogs  1.2
import QtPositioning    5.5
import QtLocation       5.6

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/component"

Item{
    id: locationCheck
    visible: false

    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Location")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Slots -------------------------------

    onVisibleChanged: {
        if(visible == false)
            return;

        mapQuickItem_SpotLocation.coordinate.latitude  = hc_LocationManager.latitude()
        mapQuickItem_SpotLocation.coordinate.longitude = hc_LocationManager.longitude()
    }

    // Map properties

    Plugin {
        id: myPlugin
        name: "osm"
    }

    // Gui ---------------------------------

    Map {
        id: map
        anchors.fill: parent

        plugin: myPlugin;
        center.latitude:  hc_LocationManager.latitude()
        center.longitude: hc_LocationManager.longitude()
        zoomLevel: 18

        Component_CurrentLocationIndicator {
        }

        MapQuickItem {
          id: mapQuickItem_SpotLocation

          anchorPoint.x: image_SpotLocation.width *0.5
          anchorPoint.y: image_SpotLocation.height

          sourceItem:Image {

              id: image_SpotLocation;
              width:  40
              height: 40
              source: "qrc:/icon/icon/find-location.png"
          }
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
            stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-upload/NearbySpotSelection.qml"),
                            properties:{stackView        : stackView}});
        }
    }


    // Message boxes -----------------------

    MessageDialog{
        id: messageDialog_LocationError
        title: qsTr('Location error')
        text: qsTr('Can\'t determine your current location in a precise way.')
    }
}
