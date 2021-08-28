
// Qt imports ------------------------------
import QtQuick 2.5
import QtQuick.Controls 1.1

Rectangle {
    id: navigator

    property alias title: text_Title.text

    property bool backButtonVisible: true
    property bool continueButtonVisible: false
    property bool menuButtonVisible: false

    property string customButtonLeftSource  : ""
    property string customButtonRightSource : ""

    signal previousPage
    signal customButtonLeftClicked
    signal customButtonRightClicked
    signal continueClicked
    signal menuClicked

    z: 2 // so flickable doesn't draw on top


    color: (   mouseArea_Back.pressed
            || mouseArea_CustomLeft.pressed
            || mouseArea_CustomRight.pressed
            || mouseArea_Menu.pressed
            || mouseArea_Continue.pressed      ) ? Qt.lighter(hc_Application.color_BackgroundNavigator(), 1.2)
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
            id: mouseArea_Back
            anchors.fill: parent
            onClicked: {
                backPressed();
            }
        }
    }

    Image {
        id: image_CustomLeft
        anchors.verticalCenter: parent.verticalCenter
        anchors.right:          image_Back.right
        height:                 parent.height
        width:                  parent.height * visible

        visible:  customButtonLeftSource != ""
        fillMode: Image.PreserveAspectFit
        smooth:   true
        source:   customButtonLeftSource
        MouseArea {
            id: mouseArea_CustomLeft
            anchors.fill: parent
            onClicked: {
                customButtonLeftClicked();
            }
        }
    }

    Image {
        id: image_CustomRight
        anchors.verticalCenter: parent.verticalCenter
        anchors.right:          image_Menu.left
        height:                 parent.height
        width:                  parent.height * visible

        visible:  customButtonRightSource != ""
        fillMode: Image.PreserveAspectFit
        smooth:   true
        source:   customButtonRightSource
        MouseArea {
            id: mouseArea_CustomRight
            anchors.fill: parent
            onClicked: {
                customButtonRightClicked();
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
            id: mouseArea_Menu
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
        MouseArea {
            id: mouseArea_Continue
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
