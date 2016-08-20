/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      17.08.2016                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.4


Item{

    // Bind properties ---------------------

    property alias text:              text_Tag.text
    property alias checked:           switch_Tag.checked
    property alias helpButtonVisible: toolButton_Help.visible


    // Signals -----------------------------

    signal helpButtonClicked


    // Gui ---------------------------------

    height: switch_Tag.height

    Text{
        id: text_Tag

        anchors.left:           parent.left
        anchors.verticalCenter: parent.verticalCenter
    }

    ToolButton{
        id: toolButton_Help

        anchors.left:           text_Tag.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        width : height

        visible: false

        Image {
            anchors.fill:    parent
            anchors.margins: 2

            smooth: true
            source: "qrc:/icon/icon/dialog-question.png"
        }

        onClicked: helpButtonClicked()
    }

    Switch{
        id: switch_Tag

        anchors.right:          parent.right
        anchors.verticalCenter: parent.verticalCenter

        checked: false
    }
}
