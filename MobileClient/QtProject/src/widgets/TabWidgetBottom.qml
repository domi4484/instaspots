
// Qt imports ------------------------------
import QtQuick 2.0

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/widgets"

Item {
    id: tabWidgetBottom

    // Properties --------------------------

    // Setting the default property to stack.children means any child items
    // of the TabWidget are actually added to the 'stack' item's children.
    // See the "Property Binding"
    // documentation for details on default properties.
    default property alias content: stack.children

    property int current: 0

    onCurrentChanged: setOpacities()
    Component.onCompleted: setOpacities()

    function setCurrentItem(item)
    {
        for (var i = 0; i < stack.children.length; ++i) {
            if(stack.children[i] === item)
            {
                current = i;
                return;
            }
        }
    }

    function setOpacities()
    {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].opacity = (i == current ? 1 : 0)
            stack.children[i].enabled = (i == current)
            stack.children[i].visible = (i == current)
        }
    }

    // Content
    Item {
        id: stack

        anchors.left:   tabWidgetBottom.left
        anchors.right:  tabWidgetBottom.right
        anchors.top:    tabWidgetBottom.top
        anchors.bottom: header.top
    }

    // Tabs
    Row {
        id: header

        anchors.bottom: tabWidgetBottom.bottom

        Repeater {
            model: stack.children.length
            delegate: TabButton {
                width: tabWidgetBottom.width / stack.children.length; height: 36
                iconSource: stack.children[index].tabWidget_ButtonIconSource
                buttonText: stack.children[index].tabWidget_ButtonText

                lineTopVisible:    true
                lineBottomVisible: false

                checked: tabWidgetBottom.current == index
                MouseArea {
                    anchors.fill: parent
                    onClicked: tabWidgetBottom.current = index
                }
            }
        }
    }
}
