import QtQuick 2.2

Rectangle {
    id: rectangle_Base
    color: "#777777"

    property bool   checked:      false
    property string image_IconSource: ""

    Rectangle {
        id: rectangle_Line
        width: parent.width
        height : 3
        color: "#7fb5be"

        anchors.top: parent.top

    }

    Image {
        id: image_Icon
        width: parent.width
        anchors.top: rectangle_Line.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 1
        fillMode: Image.PreserveAspectFit
        smooth: true
        antialiasing: true
        source: image_IconSource
    }

    onCheckedChanged: {
        if(checked)
        {
            rectangle_Base.color = "#666666"
            rectangle_Line.color = Qt.darker("#7fb5be", 1.2);
        }
        else
        {
            rectangle_Base.color = "#777777"
            rectangle_Line.color = "#7fb5be"
        }
    }
}
