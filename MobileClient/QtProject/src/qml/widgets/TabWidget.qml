
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

    property int currentIndex: 0

    property bool tabWidget_TabsOnTheBottom : false

    onCurrentIndexChanged: setOpacities()
    Component.onCompleted: {
        if(stack.children[currentIndex].tabWidget_IsDynamicTab !== null)
        {
            if(stack.children[currentIndex].tabWidgetDynamicTab_IsLoaded === false)
            {
                stack.children[currentIndex].tabWidgetDynamicTab_Load();
            }
        }

        setOpacities()
    }

    function setCurrentItem(item)
    {
        for (var i = 0; i < stack.children.length; ++i) {
            if(stack.children[i] === item)
            {
                currentIndex = i;
                return;
            }
        }
    }

    function setOpacities()
    {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].opacity = (i == currentIndex ? 1 : 0)
            stack.children[i].enabled = (i == currentIndex)
            stack.children[i].visible = (i == currentIndex)
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

                lineTopVisible:     tabWidget_TabsOnTheBottom
                lineBottomVisible: !tabWidget_TabsOnTheBottom

                checked: tabWidget.currentIndex == index
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
                        if(tabWidget.currentIndex === index)
                        {
                            if(stack.children[index].tabWidget_CurrentTabReclicked != null)
                            {
                                stack.children[index].tabWidget_CurrentTabReclicked();
                            }
                        }

                        // Set current tab content
                        tabWidget.currentIndex = index;
                    }
                }
            }
        }
    } // header
}
