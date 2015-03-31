/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      31.03.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.3

// Project imports -------------------------
import "qrc:/"

BasicPage{
    id: page_Spot

    // BasicPage properties ----------------
    continueButtonVisible: false
    menuButtonVisible: false

    // Properties --------------------------
    property int spotId

    // Signals -----------------------------
    onSpotIdChanged: {

    }

    // Gui ---------------------------------
    Column{
        id: column
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        width: parent.width / 1.1
        spacing: 5

        // GPS
        Text{
            text: qsTr("Spot id: %1").arg(spotId);
        }
    }
}
