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
    anchors.top: parent.top
    height: 40
    width: parent.width
    color: (   mouseBack.pressed
            || mouseContinue.pressed
            || mouseMenu.pressed      ) ? Qt.lighter("#7fb5be", 1.2) : "#7fb5be"

    Text {
        id: text_Title
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: "white"
        font.weight: Font.Bold
    }

    Image {
        id: image_Back
        anchors.top: parent.top
        anchors.left: parent.left
        height: parent.height
        width: parent.height * visible
        visible: backButtonVisible
        fillMode: Image.PreserveAspectFit
        source: "icon/icon/BackArrow.png"
        MouseArea {
            id: mouseBack
            anchors.fill: parent
            anchors.margins: -10
            onClicked: {
                backPressed();
            }
        }
    }

    Image {
        id: image_Menu
        anchors.top: parent.top
        anchors.right: image_Continue.left
        height: parent.height
        width: parent.height * visible
        visible: menuButtonVisible
        fillMode: Image.PreserveAspectFit
        source: "icon/icon/menu.png"
        MouseArea {
            id: mouseMenu
            anchors.fill: parent
            anchors.margins: -10
            onClicked: {
                menuClicked();
            }
        }
    }

    Image {
        id: image_Continue
        anchors.top: parent.top
        anchors.right: parent.right
        height: parent.height
        width: parent.height * visible
        visible: continueButtonVisible
        fillMode: Image.PreserveAspectFit
        source: "icon/icon/BackArrow.png"
        rotation: 180
        MouseArea {
            id: mouseContinue
            anchors.fill: parent
            anchors.margins: -10
            onClicked: {
                continueClicked();
            }
        }
    }

    function backPressed() {
        previousPage();
    }
}
