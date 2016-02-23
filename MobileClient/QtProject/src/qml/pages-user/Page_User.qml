/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      26.08.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.4
import QtQuick.Controls 1.3

// Project c++ imports ---------------------
import SpotsModel 1.0
import PicturesModel 1.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"
import "qrc:/qml/views"
import "qrc:/qml/pages-spot"
import "qrc:/qml/pages-picture"
import "qrc:/qml/component"

Item{
    id: page_User


    // Properties --------------------------

    property int userId: -1


    // Navigation properties ---------------

    property string navigation_Title:                 ""
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Models ------------------------------

    PicturesModel{
        id: picturesModel
    }

    SpotsModel{
        id: spotsModel
    }


    // Gui ---------------------------------

    TabWidgetTop {
        id: tabWidget
        width: parent.width
        height: parent.height

        Item{
            id: tab_Pictures
            anchors.fill: parent

            // Tab widget properties
            property string tabWidget_ButtonText: qsTr("Pictures(%1)").arg(picturesModel.count)
            property string tabWidget_ButtonIconSource: ""

            Component_PicturesGrid {
                anchors.fill: parent
                model: picturesModel
                onPictureClicked: {
                    stackView.push( { item: Qt.resolvedUrl("qrc:/qml/pages-picture/Page_Picture.qml"),
                                      properties : {
                                                      stackView                    : stackView,
                                                      navigation_Title             : spotName,
                                                      navigation_BackButtonVisible : true
                                                   } });
                    stackView.currentItem.model.getBy_PictureId(pictureId);
                }
            }
        }

        Item {
            id: tab_Spots
            anchors.fill: parent

            // Tab widget properties
            property string tabWidget_ButtonText: qsTr("Spots(%1)").arg(spotsModel.count)
            property string tabWidget_ButtonIconSource: ""

            Page_SpotsList {
                anchors.fill: parent
                model: spotsModel
                onSpotClicked: {
                    stackView.push( { item       : Qt.resolvedUrl("qrc:/qml/pages-spot/Page_Spot.qml"),
                                      properties : { navigation_Title             : spotName,
                                                     navigation_BackButtonVisible : true,
                                                     stackView                    : stackView,
                                                     spotId                       : spotId} });
                }
            }
        }
    }


    // Slots -------------------------------

    onUserIdChanged:
    {
        picturesModel.getBy_UserId(userId);
        spotsModel.setUserId(userId);
    }
}
