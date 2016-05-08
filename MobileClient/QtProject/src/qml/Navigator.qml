
// Qt imports ------------------------------
import QtQuick 2.2
import QtQuick.Controls 1.1

Rectangle {
    id: navigator

    property bool backButtonVisible: true
    property bool continueButtonVisible: false
    property bool menuButtonVisible: false

    property alias title: text_Title.text

    signal previousPage
    signal continueClicked
    signal menuClicked

    z: 2 // so flickable doesn't draw on top


    color: (   mouseBack.pressed
            || mouseContinue.pressed
            || mouseMenu.pressed      ) ? Qt.lighter(hc_Application.color_BackgroundNavigator(), 1.2)
                                        : hc_Application.color_BackgroundNavigator()

    Text {
        id: text_Title

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color:       "white"
        font.weight: Font.Bold
    }

    Image {
        id: image_Back
        anchors.verticalCenter: parent.verticalCenter
        anchors.left:           parent.left
        height:                 parent.height
        width:                  parent.height * visible

        visible:  backButtonVisible
        fillMode: Image.PreserveAspectFit
        smooth:   true
        source:   "qrc:/icon/icon/go-previous.png"
        MouseArea {
            id: mouseBack
            anchors.fill: parent
            onClicked: {
                backPressed();
            }
        }
    }

    Image {
        id: image_Menu
        anchors.verticalCenter: parent.verticalCenter
        anchors.right:          image_Continue.left
        height:                 parent.height - 6 * hc_Application.dip
        width:                  parent.height * visible

        visible:  menuButtonVisible
        fillMode: Image.PreserveAspectFit
        smooth:   true
        source:   "qrc:/icon/icon/view-more.png"
        MouseArea {
            id: mouseMenu
            anchors.fill: parent
            onClicked: {
                menuClicked();
            }
        }
    }

    Image {
        id: image_Continue
        anchors.verticalCenter: parent.verticalCenter
        anchors.right:          parent.right
        height:                 parent.height
        width:                  parent.height * visible

        visible:  continueButtonVisible
        fillMode: Image.PreserveAspectFit
        smooth:   true
        source:   "qrc:/icon/icon/go-next.png"
        rotation: 180
        MouseArea {
            id: mouseContinue
            anchors.fill: parent
            onClicked: {
                continueClicked();
            }
        }
    }

    function backPressed() {
        previousPage();
    }
}
