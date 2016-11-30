/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      30.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtPositioning 5.3

// Project c++ imports ---------------------
import SpotsModel 1.0

// Project qml imports ---------------------
import "qrc:/qml/views"
import "qrc:/qml/pages-spot"
import "qrc:/qml/widgets"

Item {

    SpotsModel{
        id: spotsModel
    }

    // Signals -----------------------------
    Connections {
        target: hc_LocationManager
        onSignal_Coordinate_changed:
        {
            if(visible == false)
                return;

            // ListView visible
            if(   tabWidget.currentItem == page_SpotsList
               && page_SpotsList.listView_YPosition <= 20)
            {
              page_SpotsMap.map_visibleRegion = QtPositioning.circle(hc_LocationManager.coordinate, 150*1000/2.0);
            }
        }
    }

    Connections {
        target: page_SpotsList
        onSignal_listView_atYBeginning:
        {
            page_SpotsMap.map_visibleRegion = QtPositioning.circle(hc_LocationManager.coordinate, 150*1000/2.0);
        }
    }

    // Slots -------------------------------
    Component.onCompleted: {
        // Set current location (also if outdated)
        page_SpotsMap.map_visibleRegion = QtPositioning.circle(hc_LocationManager.coordinate, 150*1000/2.0);
    }

    onVisibleChanged: {
        if(visible == false)
        {
            hc_LocationManager.stopUdates();
            return;
        }

        // Set current location (also if outdated)
        page_SpotsMap.map_center = hc_LocationManager.coordinate;

        // Request location update
        hc_LocationManager.startUpdates();
    }


    // Gui ---------------------------------

    Navigator{
        id: navigator

        anchors.top: parent.top
        height:      30 * hc_Application.dip
        width:       parent.width

        title                 : (stackView.currentItem != null) ? stackView.currentItem.navigation_Title                 : "";
        backButtonVisible     : stackView.depth > 1
        continueButtonVisible : (stackView.currentItem != null) ? stackView.currentItem.navigation_ContinueButtonVisible : false;
        menuButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_MenuButtonVisible     : false;

        onPreviousPage: {
            if(stackView.depth > 1)
                stackView.pop();
        }

        onContinueClicked: {
            if(stackView.currentItem == null)
                return;

            if(stackView.currentItem.navigator_ContinueButtonClicked != null)
                stackView.currentItem.navigator_ContinueButtonClicked();
        }

        onMenuClicked: {
            if(stackView.currentItem == null)
                return;

            if(stackView.currentItem.navigator_MenuButtonClicked != null)
                stackView.currentItem.navigator_MenuButtonClicked();
        }
    } // Navigator

    StackView {
        id: stackView

        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.top:    navigator.bottom
        anchors.bottom: parent.bottom

        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                                 stackView.pop();
                                 event.accepted = true;
                             }

        initialItem: TabWidget {
            id: tabWidget

            // Navigation properties ---------------
            property string navigation_Title:                 qsTr('Nearby spots')
            property bool   navigation_BackButtonVisible:     false
            property bool   navigation_ContinueButtonVisible: false
            property bool   navigation_MenuButtonVisible:     false

            width : parent.width
            height: parent.height

            Page_SpotsList {

                id: page_SpotsList

                // TabWidget properties
                property string tabWidget_ButtonText: qsTr("List")
                property string tabWidget_ButtonIconSource: ""

                anchors.fill: parent


                model: spotsModel

                onSpotClicked: {
                    stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-spot/Page_Spot.qml"),
                                       properties:{navigation_Title : spotName,
                                                   stackView        : stackView,
                                                   spotId           : spotId}});
                }
            } // Page_SpotsList

            Page_SpotsMap {

                id: page_SpotsMap

                property string tabWidget_ButtonText: qsTr("Map")
                property string tabWidget_ButtonIconSource: ""

                anchors.fill: parent

                model:      spotsModel

                onSpotClicked: {
                    stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-spot/Page_Spot.qml"),
                                       properties:{navigation_Title : spotName,
                                                   stackView        : stackView,
                                                   spotId           : spotId}});
                }

                onMap_centerChanged: {
                    page_SpotsMap_VisibleRegion_Changed(map_visibleRegion);
                }

                onMap_zoomLevelChanged: {
                    page_SpotsMap_VisibleRegion_Changed(map_visibleRegion);
                }
            } // Page_SpotsMap
        } // Tabwidget
    } // StackView


    // Functions ---------------------------

    function page_SpotsMap_VisibleRegion_Changed(visibleRegion)
    {
      spotsModel.updateBy_VisibleRegion(visibleRegion);
    }
}
