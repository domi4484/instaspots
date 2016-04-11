/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      01.04.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.3

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/views"

Item{
    id: page_SpotsList

    // Navigation properties ---------------

    property string navigation_Title:                 ""
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Bind properties ---------------------

    property alias model: listView.model


    // Signals -----------------------------

    signal spotClicked(int    spotId,
                       string spotName,
                       string spotDescription)

    // Gui ---------------------------------

    ListView {
        id: listView
        anchors.fill: parent
        delegate: SpotOverviewDelegate{

            onSpotClicked: {
                page_SpotsList.spotClicked(role_SpotId,
                                           role_SpotName,
                                           role_SpotDescription);
            }
        }
    }
}

