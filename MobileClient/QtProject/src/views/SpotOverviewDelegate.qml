/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      02.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.0
import QtQuick.Controls 1.2

// Project imports -------------------------
import "qrc:/widgets"

Item {
    id: root
    width: parent.width
    height:   rectangle_Top.height
            + image_Picture1.height

    signal spotClicked

    Rectangle {
        id: rectangle_Top
        anchors.top: parent.top
        width: parent.width
        height: 40
        color: "#aaaaaa"
        Text{
            id: text_SpotName
            anchors.leftMargin: 2
            width: parent.width / 2
            height: parent.height / 2
            text: role_SpotName
        }
        Text {
            anchors.top: parent.top
            anchors.left: text_SpotName.right
            anchors.rightMargin: 2
            width: text_SpotName.width
            height: text_SpotName.height
            horizontalAlignment: Text.AlignRight
            text: role_SpotDistance
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                spotClicked();
            }
        }
    }
    CachedPicture {
        id: image_Picture1
        anchors.top: rectangle_Top.bottom
        width: parent.width/2
        height: parent.width/2
        sourceUrl: role_SpotPictureUrl1
        MouseArea {
            anchors.fill: parent
            onClicked: {
                spotClicked();
            }
        }
    }
    CachedPicture {
        id: image_Picture2
        anchors.top: rectangle_Top.bottom
        anchors.left: image_Picture1.right
        width: parent.width/2
        height: parent.width/2
        sourceUrl: role_SpotPictureUrl2
        MouseArea {
            anchors.fill: parent
            onClicked: {
                spotClicked();
            }
        }
    }
}

