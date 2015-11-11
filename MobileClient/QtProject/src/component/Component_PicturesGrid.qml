/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      11.11.2015                                        *
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
import "qrc:/views"
import "qrc:/widgets"

Item{
    id: component_PicturesGrid


    // Bind properties ---------------------

    property alias model: gridView_Pictures.model


    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Pictures")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Signals -----------------------------

    signal pictureClicked(int pictureId)


    // Gui ---------------------------------

    GridView{
        id: gridView_Pictures
        anchors.fill: parent

        cellWidth: parent.width / 2
        cellHeight: cellWidth

        delegate: component_Picture
    }


    // Components --------------------------

    Component {
        id: component_Picture

        Item {
            width: gridView_Pictures.cellWidth
            height: gridView_Pictures.cellHeight

            CachedPicture {
                id: image_Picture
                anchors.fill: parent
                sourceUrl: role_PictureUrl
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        component_PicturesGrid.pictureClicked(role_PictureId);
                    }
                }
            }
        }
    }
}

