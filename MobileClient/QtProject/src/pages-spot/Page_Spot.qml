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

Item{
    id: page_Spot


    // Properties --------------------------

    property Navigator navigator
    property StackView stackView


    // Bind properties ---------------------

    property alias model: picturesModel


    // Navigation properties ---------------

    property string navigation_Title:                 "-"
    property bool   navigation_BackButtonVisible:     false
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Models ------------------------------

    PicturesModel{
        id: picturesModel
    }

    // Components --------------------------

    Component {
        id: component_Picture

        Item {
            signal pictureClicked

            width: gridView_Pictures.cellWidth
            height: gridView_Pictures.cellHeight

            CachedPicture {
                id: image_Picture
                anchors.fill: parent
                sourceUrl: role_PictureUrl
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var page_PicturesList = Qt.resolvedUrl("qrc:/pages-picture/Page_PicturesList.qml");
                        stackView.push({item: page_PicturesList,
                                        properties : {width                        : page_Spot.width,
                                                      height                       : page_Spot.height,
                                                      navigation_Title             : navigation_Title,
                                                      navigation_BackButtonVisible : true}});
                        stackView.currentItem.model.getBy_PictureId(role_PictureId);
                    }
                }
            }
        }
    }


    // Gui ---------------------------------

    Item{
        id: item_Gui
        width: parent.width
        height: parent.height

        GridView{
            id: gridView_Pictures
            anchors.fill: parent

            cellWidth: parent.width / 2
            cellHeight: cellWidth

            model: picturesModel
            delegate: component_Picture
        }
    }
}
