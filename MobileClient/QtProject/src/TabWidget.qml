import QtQuick 2.0

Item {
    id: tabWidget

    // Setting the default property to stack.children means any child items
    // of the TabWidget are actually added to the 'stack' item's children.
    // See the "Property Binding"
    // documentation for details on default properties.
    default property alias content: stack.children

    property int current: 0

    onCurrentChanged: setOpacities()
    Component.onCompleted: setOpacities()

    function setOpacities() {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].opacity = (i == current ? 1 : 0)
            stack.children[i].enabled = (i == current)
            stack.children[i].visible = (i == current)
        }
    }

    Item {
        id: stack
        width: tabWidget.width
        anchors.top: tabWidget.top
        anchors.bottom: header.top

    }

    Row {
        id: header

        anchors.bottom: tabWidget.bottom

        Repeater {
            model: stack.children.length
            delegate: TabButton {
                width: tabWidget.width / stack.children.length; height: 36
                image_IconSource: stack.children[index].image_IconSource
                checked: tabWidget.current == index
                MouseArea {
                    anchors.fill: parent
                    onClicked: tabWidget.current = index
                }
            }
        }
    }
}
