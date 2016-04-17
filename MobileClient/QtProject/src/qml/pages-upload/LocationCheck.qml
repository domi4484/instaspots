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

    property string navigation_Title:                 qsTr("Spot location")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


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
        zoomLevel: 18

        Component.onCompleted: {
            center = hc_LocationManager.coordinate

            mapQuickItem_SpotLocation.coordinate.latitude  = hc_LocationManager.latitude()
            mapQuickItem_SpotLocation.coordinate.longitude = hc_LocationManager.longitude()
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                mapQuickItem_SpotLocation.coordinate = map.toCoordinate(Qt.point(mouseX, mouseY), false);
            }
        }

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
            wa_PictureUploader.coordinate = mapQuickItem_SpotLocation.coordinate;
            stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-upload/NearbySpotSelection.qml"),
                            properties:{stackView        : stackView}});
        }
    }
}
