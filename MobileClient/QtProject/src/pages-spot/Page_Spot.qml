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
import "qrc:/widgets"

BasicPage{
    id: page_Spot

    // BasicPage properties ----------------
    continueButtonVisible: false
    menuButtonVisible: false

    // Properties --------------------------
    property int spotId
    property Navigator navigator
    property StackView stackView

    // Signals -----------------------------
    onSpotIdChanged:
    {
        wa_PicturesModel.setSpotId(spotId);
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
        width: parent.width
        height: parent.height

        Component.onCompleted:
        {
            console.log("Width: ", width, "Height: ", height);
        }

        GridView{
            id: gridView_Pictures
            anchors.fill: parent

            cellWidth: parent.width / 2
            cellHeight: cellWidth

            model: wa_PicturesModel
            delegate: component_Picture
        }
    }
}
