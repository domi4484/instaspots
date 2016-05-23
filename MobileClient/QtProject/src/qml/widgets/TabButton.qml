
// Qt imports ------------------------------
import QtQuick 2.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"

Rectangle {
    id: rectangle_Base
    color: "#777777"


    // Properties --------------------------

    property bool   checked:      false
    property string iconSource: ""
    property string buttonText: ""

    property alias lineTopVisible:    rectangle_LineTop.visible
    property alias lineBottomVisible: rectangle_LineBottom.visible


    // Gui ---------------------------------

    Rectangle {
        id: rectangle_LineTop
        width: parent.width
        height : 3
        anchors.top: parent.top

        color:   "#7fb5be"
    }

    Image {
        id: image_Icon
        width: parent.width
        anchors.top: rectangle_LineTop.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 1

        fillMode:     Image.PreserveAspectFit
        smooth:       true
        antialiasing: true
        source:       iconSource
    }

    Text {
        id: rectangle_Text
        width: parent.width
        anchors.top:     rectangle_LineTop.bottom
        anchors.bottom:  rectangle_LineBottom.top
        anchors.margins: 1

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:   Text.AlignVCenter
        color: "white"
        text: buttonText
    }

    Rectangle {
        id: rectangle_LineBottom
        width: parent.width
        height : 3
        anchors.bottom: parent.bottom

        color: "#7fb5be"
    }


    // Slots -------------------------------

    onCheckedChanged: {
        if(checked)
        {
            rectangle_Base.color = "#666666"
            rectangle_LineTop.color = Qt.darker("#7fb5be", 1.2);
        }
        else
        {
            rectangle_Base.color = "#777777"
            rectangle_LineTop.color = "#7fb5be"
        }
    }
}
