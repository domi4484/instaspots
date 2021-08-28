
// Qt imports ------------------------------
import QtQuick 2.5
import QtQuick.Controls 1.0

// Project qml imports ---------------------
import "qrc:/qml/widgets"

Item {
    id: root
    width: parent.width
    height:   rectangle_Top.height
            + image_Picture.height
            + rectangle_Bottom.height
            + 20 * hc_Application.dip


    signal userClicked
    signal spotClicked
    signal optionsClicked
    signal likesClicked

    Rectangle {
        id: rectangle_Top

        anchors.top: parent.top
        width: parent.width

        height: link_Username.height * 2

        // Title
        Link {
            id: link_SpotName
            anchors.fill: parent

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:   Text.AlignVCenter

            text:  role_SpotName

            onClicked: spotClicked()
        }

        // Username
        Link{
            id: link_Username

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            verticalAlignment: Text.AlignTop

            text: role_UserUsername

            onClicked: userClicked()
        }

        // Options
        ToolButton {
            id: toolButton_Options

            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right

            height: parent.height
            width:  parent.height

            Image {
                anchors.fill:    parent
                anchors.margins: 2

                smooth: true
                source: "qrc:/icon/icon/view-more.png"
            }

            onClicked: optionsClicked()
        }
    } // Rectangle

    // Picture
    CachedPicture {
        id: image_Picture

        anchors.top: rectangle_Top.bottom
        width: parent.width
        height: width

        sourceUrl: role_PictureUrl

        MouseArea {
            anchors.fill: parent
            onClicked: spotClicked()
        }
    }

    Rectangle {
        id: rectangle_Bottom

        anchors.top: image_Picture.bottom
        width: parent.width

        height: link_Likes.height * 2

        // Likes
        Link{
            id: link_Likes

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            text: picturesModel.getPicture(index).likersCount + " " + qsTr("Likes")

            onClicked: likesClicked()
        } // Likes
    } // Rectangle
}
