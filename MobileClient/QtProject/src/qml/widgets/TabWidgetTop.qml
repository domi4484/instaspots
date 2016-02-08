
// Qt imports ------------------------------
import QtQuick 2.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"

Item {
    id: tabWidgetTop

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

    // Tabs
    Row {
        id: header

        anchors.top: tabWidgetTop.top

        Repeater {
            model: stack.children.length
            delegate: TabButton {
                width: tabWidgetTop.width / stack.children.length; height: 36
                iconSource: stack.children[index].tabWidget_ButtonIconSource
                buttonText: stack.children[index].tabWidget_ButtonText

                lineTopVisible:    false
                lineBottomVisible: true

                checked: tabWidgetTop.current == index
                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {
                        // Re-Click
                        if(tabWidgetTop.current === index)
                        {
                            if(stack.children[index].tabWidget_CurrentTabReclicked != null)
                            {
                                stack.children[index].tabWidget_CurrentTabReclicked();
                            }
                        }

                        // Set current tab content
                        tabWidgetTop.current = index;
                    }
                }
            }
        }
    }

    // Content
    Item {
        id: stack

        anchors.left:   tabWidgetTop.left
        anchors.right:  tabWidgetTop.right
        anchors.top:    header.bottom
        anchors.bottom: tabWidgetTop.bottom
    }
}
