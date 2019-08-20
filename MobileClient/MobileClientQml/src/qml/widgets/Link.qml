/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      23.05.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.5

Text {

    id: link

    // Signals -----------------------------

    signal clicked()


    color:     mouseArea.pressed ? hc_Application.color_TextLink_Pressed() : hc_Application.color_TextLink()
    font.bold: true


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            link.clicked();
        }
    }
}
