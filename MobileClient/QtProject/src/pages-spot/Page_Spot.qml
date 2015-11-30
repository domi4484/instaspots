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

// Project c++ imports ---------------------
import PicturesModel 1.0

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/widgets"
import "qrc:/pages-picture"
import "qrc:/component"

Item{
    id: page_Spot


    // Properties --------------------------

    property int       spotId: -1


    // Navigation properties ---------------

    property string navigation_Title:                 picturesModel.data(picturesModel.index(0), role_SpotName)
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Models ------------------------------

    PicturesModel{
        id: picturesModel
    }


    // Gui ---------------------------------

    Component_PicturesGrid {
        id: component_PicturesGrid

        anchors.fill: parent
        model: picturesModel
        onPictureClicked: {
            stackView.push( { item: Qt.resolvedUrl("qrc:/pages-picture/Page_Picture.qml"),
                              properties : {
                                              stackView                    : stackView,
                                              navigation_BackButtonVisible : true
                                           } });
            stackView.currentItem.model.getBy_PictureId(pictureId);
        }
    }


    // Signals -----------------------------

    onSpotIdChanged:
    {
        picturesModel.getBy_SpotId(spotId);
    }
}
