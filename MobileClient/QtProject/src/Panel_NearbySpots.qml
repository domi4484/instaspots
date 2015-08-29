/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      30.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.0
import QtQuick.Controls 1.1

// Project c++ imports ---------------------
import NearbySpotsModel 1.0

// Project imports -------------------------
import "qrc:/views"
import "qrc:/pages-spot"

Item {

    NearbySpotsModel{
        id: nearbySpotsModel
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

            nearbySpotsModel.setLocation(hc_LocationManager.latitude(),
                                         hc_LocationManager.longitude(),
                                         150);
        }
    }

    // Slots -------------------------------
    onVisibleChanged: {
        if(visible == false)
            return;

        // Set current location (also if outdated)
        nearbySpotsModel.setLocation(hc_LocationManager.latitude(),
                                     hc_LocationManager.longitude(),
                                     150);

        // Request location update
        hc_LocationManager.requestLocation();
    }

    // Gui ---------------------------------
    Navigator{
        id: navigator
        anchors.top: parent.top

        backButtonVisible: stackView.depth > 1
        continueButtonVisible: stackView.currentItem.continueButtonVisible

        onPreviousPage: {
            if(stackView.depth > 1)
                stackView.pop();
        }

        onContinueClicked: {
            stackView.currentItem.continueClicked();
        }
    }

    StackView {
        id: stackView
        anchors.top: navigator.bottom
        anchors.bottom: parent.bottom
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true;
                         }
        initialItem: BasicPage {
            id: page_Locating
            width: parent.width
            height: parent.height

            title: qsTr('Nearby spots')

            Page_SpotsList {
                width: parent.width
                height: parent.height
                model: nearbySpotsModel
                onSpotClicked: {
                    stackView.push({item: Qt.resolvedUrl("pages-spot/Page_Spot.qml"),
                                   properties:{title:spotName,
                                               width:stackView.width,
                                               height:stackView.height,
                                               stackView:stackView,
                                               navigator:navigator,
                                               spotId:spotId}});
                }
            }
        }

        onCurrentItemChanged: {
            if(currentItem == null)
                return;

            navigator.title = currentItem.title;
            navigator.continueButtonVisible = currentItem.continueButtonVisible;
        }
    }
}
