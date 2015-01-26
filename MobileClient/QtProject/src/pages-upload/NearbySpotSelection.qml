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

// Project imports -------------------------
import "qrc:/"
import "qrc:/views"

BasicPage{
    id: nearbySpotSelection

    // BasicPage properties ----------------
    title: qsTr("Spot selection")
    continueButtonVisible: false

    // Signals -----------------------------
    signal addNewSpot()

    // Gui ---------------------------------
    Button{
        id: button_AddNewSpot
        z: 2
        width: parent.width / 1.1
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("Add new spot")

        onClicked:{
            wa_PictureUploader.setNewSpot(true);

            addNewSpot();
        }
    }

    ListView {
        width: parent.width
        anchors.top: button_AddNewSpot.bottom
        anchors.bottom: parent.bottom
        model: wa_NearbySpotModel
        delegate: SpotOverviewDelegate{
        }
    }
}

