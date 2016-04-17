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
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

// Project imports -------------------------
import "qrc:/qml/widgets"

Rectangle {
    id: root
    width: parent.width
    height:   rectangle_Top.height
            + image_Picture1.height
            + 15

    signal spotClicked

    Rectangle {
        id: rectangle_Top

        anchors.top:         parent.top
        anchors.leftMargin:  2
        anchors.rightMargin: 2
        width:  parent.width
        height: text_SpotName.height

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

            color:     hc_Application.color_TextLink()
            text:      role_SpotName
            font.bold: true
        }


        // Distance
        Text{
            id: text_SpotDistance

            anchors.right: parent.right
            width: parent.width / 2
            height: parent.height

            horizontalAlignment: Text.AlignRight
            text:      role_SpotDistance

            color:     hc_Application.color_TextLink()
            font.bold: true

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(role_SpotSecretSpot === false)
                    {
                        hc_LocationManager.openLocationOnNativeMapsApp(role_SpotLatitude,
                                                                       role_SpotLongitude,
                                                                       role_SpotName);
                    }
                    else
                    {
                        messageDialog_SecretSpotClicked.open();
                    }
                }
            }
        }
    }
    CachedPicture {
        id: image_Picture1
        anchors.top: rectangle_Top.bottom
        width: parent.width/2 - 1
        height: parent.width/2 - 1
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
        anchors.right: parent.right
        width: parent.width/2 - 1
        height: parent.width/2 - 1
        sourceUrl: role_SpotPictureUrl2
        MouseArea {
            anchors.fill: parent
            onClicked: {
                spotClicked();
            }
        }
    }

    // Message dialogs ---------------------
    MessageDialog{
        id: messageDialog_SecretSpotClicked
        title: qsTr('Secret spot')
        text: qsTr('The precise location of a secret spot is not displayed. To find the spot try to ask some local skaters.')
    }
}
