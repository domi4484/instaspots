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

// Project imports -------------------------
import "qrc:/"
import "qrc:/views"

BasicPage{
    id: page_SpotsList

    // BasicPage properties ----------------
    title: qsTr("Pictures")
    continueButtonVisible: false
    menuButtonVisible: false

    // Bind properties ---------------------
    property alias model: listView.model

    // Signals -----------------------------
    signal spotClicked(int spotId, string spotName, string spotDescription)

    onVisibleChanged: {
        if(visible == false)
        {
            return;
        }

        wa_NearbySpotModel.setLocation(hc_LocationManager.latitude(),
                                       hc_LocationManager.longitude());
    }

    // Gui ---------------------------------
    ListView {
        id: listView
        anchors.fill: parent
        delegate: SpotOverviewDelegate{

            onSpotClicked: {
                page_SpotsList.spotClicked(role_SpotId, role_SpotName, role_SpotDescription);
            }
        }
    }
}

