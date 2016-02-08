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

// Project c++ imports ---------------------
import SpotsModel 1.0

// Project qml imports ---------------------
import "qrc:/qml/views"
import "qrc:/qml/pages-spot"

Item {

    SpotsModel{
        id: spotsModel
    }

    // Signals -----------------------------
    Connections {
        target: hc_LocationManager
        onUpdate:
        {
            if(visible == false)
                return;

            if(timeout)
            {
                // TODO display message cacca
            }

            spotsModel.setLocation(hc_LocationManager.latitude(),
                                   hc_LocationManager.longitude(),
                                   150);
        }
    }

    // Slots -------------------------------
    onVisibleChanged: {
        if(visible == false)
            return;

        // Set current location (also if outdated)
        spotsModel.setLocation(hc_LocationManager.latitude(),
                               hc_LocationManager.longitude(),
                               150);

        // Request location update
        hc_LocationManager.requestLocation();
    }


    // Gui ---------------------------------

    Navigator{
        id: navigator

        anchors.top: parent.top
        width:       parent.width
        height:      40

        title                 : (stackView.currentItem != null) ? stackView.currentItem.navigation_Title                 : "";
        backButtonVisible     : stackView.depth > 1
        continueButtonVisible : (stackView.currentItem != null) ? stackView.currentItem.navigation_ContinueButtonVisible : false;
        menuButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_MenuButtonVisible     : false;

        onPreviousPage: {
            if(stackView.depth > 1)
                stackView.pop();
        }
    }

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
        initialItem: Page_SpotsList {
            width : parent.width
            height: parent.height

            navigation_Title: qsTr('Nearby spots')

            model: spotsModel

            onSpotClicked: {
                stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-spot/Page_Spot.qml"),
                                properties:{navigation_Title : spotName,
                                            stackView        : stackView,
                                            spotId           : spotId}});
            }
        }
    }
}
