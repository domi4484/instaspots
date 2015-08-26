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

BasicPage{
    id: page_Spot

    // BasicPage properties ----------------
    continueButtonVisible: false
    menuButtonVisible: false

    // Properties --------------------------
    property int spotId: -1
    property Navigator navigator
    property StackView stackView

    PicturesModel{
        id: picturesModel
    }

    // Signals -----------------------------
    onSpotIdChanged:
    {
        picturesModel.setSpotId(spotId)
    }

    // Connections -------------------------

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
                        stackView.push({item: Qt.resolvedUrl("qrc:/widgets/CachedPicture.qml"),
                                       properties:{width:stackView.width,
                                                   height:width,
                                                   sourceUrl:role_PictureUrl}});
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
