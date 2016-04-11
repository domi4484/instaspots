/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      11.04.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.0
import QtPositioning 5.5
import QtLocation 5.6

Item {
    id: page_SpotsMap

    // Navigation properties ---------------

    property string navigation_Title:                 ""
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Bind properties ---------------------

    property alias model: mapItemView.model


    // Signals -----------------------------

    signal spotClicked(int    spotId,
                       string spotName,
                       string spotDescription)


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

        MapItemView {
            id: mapItemView

            delegate: MapQuickItem {
              //  coordinate: place.location.coordinate

                coordinate.latitude: role_SpotLatitude
                coordinate.longitude:  role_SpotLongitude

                anchorPoint.x: image.width * 0.5
                anchorPoint.y: image.height

                sourceItem: Column {
                    Image {
                        id: image;
                        width:  40
                        height: 40
                        source: "qrc:/icon/icon/find-location.png"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                    page_SpotsMap.spotClicked(role_SpotId,
                                                              role_SpotName,
                                                              role_SpotDescription);
                            }
                        }
                    }
                  //  Text {
                  //      text: role_SpotName;
                  //      font.bold: false
                  //  }
                }
            }
        }
    }
}
