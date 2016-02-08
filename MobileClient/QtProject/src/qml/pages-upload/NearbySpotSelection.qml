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

// Project c++ imports ---------------------
import SpotsModel 1.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/views"
import "qrc:/qml/pages-spot/"

Item{
    id: nearbySpotSelection

    SpotsModel{
        id: spotsModel
    }


    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Spot selection")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Signals -----------------------------

    signal addNewSpot()
    signal addToExistingSpot()


    // Connections -------------------------

    onVisibleChanged: {
        if(visible == false)
        {
            return;
        }

        console.log("nearbySpotModel.setLocation")

        spotsModel.setLocation(hc_LocationManager.latitude(),
                               hc_LocationManager.longitude(),
                               4);
    }


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

    Page_SpotsList {
        width: parent.width
        anchors.top: button_AddNewSpot.bottom
        anchors.bottom: parent.bottom
        model: spotsModel
        onSpotClicked: {
            wa_PictureUploader.setNewSpot(false);
            wa_PictureUploader.setExistingSpotId(spotId);
            wa_PictureUploader.setName(spotName);
            wa_PictureUploader.setDescription(spotDescription);
            addToExistingSpot();
        }
    }
}

