
// Qt imports ------------------------------
import QtQuick 2.1
import QtQuick.Controls 1.0

// Project qml imports ---------------------
import "qrc:/qml/widgets"

Item {
    id: root
    width: parent.width
    height:   rectangle_Top.height
            + image_Picture.height

    signal userClicked
    signal spotClicked

    Rectangle {
        id: rectangle_Top

        anchors.top: parent.top
        width: parent.width
        height: 40

        color: hc_Application.color_BackgroundTitleRectangle()

        // Username
        Text{
            id: text_Username
            width: parent.width / 2
            height: parent.height / 2

            text:      role_UserUsername
            color:     hc_Application.color_TextLink()
            font.bold: true

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    userClicked();
                }
            }
        }

        // Time
        Text {
            anchors.top: parent.top
            anchors.left: text_Username.right
            width: text_Username.width
            height: text_Username.height
            horizontalAlignment: Text.AlignRight

            text: role_PictureCreated
        }

        // Title
        Text {
            id: text_SpotName
            anchors.top: text_Username.bottom
            width: parent.width
            height: text_Username.height
            horizontalAlignment: Text.AlignHCenter

            text: role_SpotName
        }
    }

    // Picture
    CachedPicture {
        id: image_Picture
        anchors.top: rectangle_Top.bottom
        width: parent.width
        height: width

        sourceUrl: role_PictureUrl

        MouseArea {
            anchors.fill: parent
            onClicked: {
                spotClicked();
            }
        }
    }
}
