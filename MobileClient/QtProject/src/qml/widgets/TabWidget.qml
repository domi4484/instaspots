
// Qt imports ------------------------------
import QtQuick 2.0
import QtQuick.Window 2.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"

Item {
    id: tabWidget

    // Properties --------------------------

    // Setting the default property to stack.children means any child items
    // of the TabWidget are actually added to the 'stack' item's children.
    // See the "Property Binding"
    // documentation for details on default properties.
    default property alias content: stack.children

    property int current: 0

    property bool tabWidget_TabsOnTheBottom : false

    onCurrentChanged: setOpacities()
    Component.onCompleted: {
        if(stack.children[current].tabWidget_IsDynamicTab !== null)
        {
            if(stack.children[current].tabWidgetDynamicTab_IsLoaded === false)
            {
                stack.children[current].tabWidgetDynamicTab_Load();
            }
        }

        setOpacities()
    }

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


    Item {
        id: stack

        height: tabWidget.height - header.height
        width : parent.width

        anchors.top: tabWidget_TabsOnTheBottom ? tabWidget.top : header.bottom
    } // stack

    // Tabs
    Row {
        id: header

        height: 30 * hc_Application.dip
        width:  parent.width

        anchors.top : tabWidget_TabsOnTheBottom ? stack.bottom : tabWidget.top

        Repeater {
            model: stack.children.length
            delegate: TabButton {
                height: 30 * hc_Application.dip
                width:  parent.width / stack.children.length;
                iconSource: stack.children[index].tabWidget_ButtonIconSource
                buttonText: stack.children[index].tabWidget_ButtonText

                lineTopVisible:    true
                lineBottomVisible: false

                checked: tabWidget.current == index
                MouseArea {
                    anchors.fill: parent
                    onClicked:
                    {
                        // Children to load dinamically?
                        if(stack.children[index].tabWidget_IsDynamicTab !== null)
                        {
                            if(stack.children[index].tabWidgetDynamicTab_IsLoaded === false)
                            {
                                stack.children[index].tabWidgetDynamicTab_Load();
                            }
                        }

                        // Click
                        if(stack.children[index].tabWidget_CurrentTabClicked != null)
                        {
                            stack.children[index].tabWidget_CurrentTabClicked();
                        }

                        // Re-Click
                        if(tabWidget.current === index)
                        {
                            if(stack.children[index].tabWidget_CurrentTabReclicked != null)
                            {
                                stack.children[index].tabWidget_CurrentTabReclicked();
                            }
                        }

                        // Set current tab content
                        tabWidget.current = index;
                    }
                }
            }
        }
    } // header
}
