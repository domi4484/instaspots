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

        mapQuickItem_CurrentLocation.coordinate.latitude  = hc_LocationManager.latitude()
        mapQuickItem_CurrentLocation.coordinate.longitude = hc_LocationManager.longitude()

        mapQuickItem_SpotLocation.coordinate.latitude  = hc_LocationManager.latitude()
        mapQuickItem_SpotLocation.coordinate.longitude = hc_LocationManager.longitude()
    }

    Connections {
        target: hc_LocationManager
        onUpdate:
        {
            if(visible == false)
                return;

            if(timeout)
                return;

            mapQuickItem_CurrentLocation.coordinate.latitude  = hc_LocationManager.latitude()
            mapQuickItem_CurrentLocation.coordinate.longitude = hc_LocationManager.longitude()
        }
    }

    // Map properties

    Plugin {
        id: myPlugin
        name: "osm"
        //specify plugin parameters if necessary
        //PluginParameter {...}
        //PluginParameter {...}
        //...
    }

    // Gui ---------------------------------

    Map {
        id: map
        anchors.fill: parent

        plugin: myPlugin;
        center.latitude: hc_LocationManager.latitude()
        center.longitude: hc_LocationManager.longitude()
        zoomLevel: 6

        MapQuickItem {
          id: mapQuickItem_CurrentLocation

          anchorPoint.x: image_CurrentLocation.width *0.5
          anchorPoint.y: image_CurrentLocation.height*0.5

          sourceItem:Image {

              id: image_CurrentLocation;
              width:  18
              height: 18

              SequentialAnimation {
                  loops: Animation.Infinite
                  running: true
                  OpacityAnimator {
                      target: image_CurrentLocation;
                      from: 1;
                      to:   0.2;
                      duration: 1500
                  }
                  OpacityAnimator {
                      target: image_CurrentLocation;
                      from: 0.2;
                      to:   1;
                      duration: 1500
                  }
              }

              source: "qrc:/icon/icon/Wheel-29x29.png"
          }
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
