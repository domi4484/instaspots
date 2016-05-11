/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      11.04.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick       2.5
import QtPositioning 5.5
import QtLocation    5.6

// Project qml imports ---------------------
import "qrc:/qml/component"

Item {
    id: page_SpotsMap

    // Navigation properties ---------------

    property string navigation_Title:                 ""
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Bind properties ---------------------

    property alias model: mapItemView.model

    property alias map_center: map.center


    // Signals -----------------------------

    signal spotClicked(int    spotId,
                       string spotName,
                       string spotDescription)


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
        zoomLevel: 8

        Component.onCompleted: {
            center = hc_LocationManager.coordinate
        }

        Component_CurrentLocationIndicator {
        }

        MapItemView {
            id: mapItemView

            delegate: MapQuickItem {

                coordinate.latitude:   role_SpotLatitude
                coordinate.longitude:  role_SpotLongitude

                anchorPoint.x: image.width * 0.5
                anchorPoint.y: image.height

                sourceItem: Column {

                    visible: !role_SpotSecretSpot

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
                    Text {
                        text: role_SpotName;
                        font.bold: true
                        visible: map.zoomLevel > 15
                    }
                }
            }
        }
    }
}
