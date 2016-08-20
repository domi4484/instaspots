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
import Spot          1.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"
import "qrc:/qml/pages-picture"
import "qrc:/qml/component"

Item{
    id: page_Spot


    // Properties --------------------------

    property int       spotId: -1
    property Spot      spot


    // Navigation properties ---------------

    property string navigation_Title:                 ""
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Models ------------------------------

    PicturesModel{
        id: picturesModel
    }


    // Gui ---------------------------------

    Rectangle {
        id: rectangle_Top

        anchors.top: parent.top
        width: parent.width - 8
        x: 4
        height: text_SpotName.height *2

        MouseArea {
            anchors.fill: parent
            onClicked: {
                spotClicked();
            }
        }

        // SpotName
        Text{
            id: text_SpotName

            width: parent.width / 2
            anchors.centerIn: parent

            text: spot.name

            color:     hc_Application.color_TextLink()
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }


        // Distance
        Link{
            id: link_SpotDistance

            anchors.right: parent.right
            width: parent.width / 2
            height: parent.height

            text: spot.distanceText

            horizontalAlignment: Text.AlignRight

            onClicked: {
                if(spot.secretSpot === false)
                {
                    hc_LocationManager.openLocationOnNativeMapsApp(spot.latitude,
                                                                   spot.longitude,
                                                                   spot.name);
                }
                else
                {
                    messageDialog_SecretSpotClicked.open();
                }
            }
        }
    }

    Component_PicturesGrid {
        id: component_PicturesGrid

        anchors.top: rectangle_Top.bottom
        height: parent.height
        width:  parent.width

        model: picturesModel
        onPictureClicked: {
            stackView.push( { item: Qt.resolvedUrl("qrc:/qml/pages-picture/Page_Picture.qml"),
                              properties : {
                                              stackView                    : stackView,
                                              navigation_Title             : page_Spot.navigation_Title,
                                              navigation_BackButtonVisible : true
                                           } });
            stackView.currentItem.model.getBy_PictureId(pictureId);
        }
    }


    // Signals -----------------------------

    onSpotIdChanged:
    {
        spot = re_SpotRepository.getBy_SpotId(spotId);
        picturesModel.getBy_SpotId(spotId);
    }
}
