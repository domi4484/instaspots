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

// Project imports -------------------------
import "qrc:/views"

Item {

    // Slots -------------------------------
    onVisibleChanged: {
        if(visible == false)
        {
            return;
        }

        if(hc_PlateformDetail.isMobile())
        {
            wa_NearbySpotModel.setLocation(hc_LocationManager.latitude(),
                                           hc_LocationManager.longitude());
        }
        else
        {
            // Host Bellinzona
            wa_NearbySpotModel.setLocation(46.194755,
                                           9.027456);
        }
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

            ListView {
                width: parent.width
                height: parent.height
                model: wa_NearbySpotModel
                delegate: SpotOverviewDelegate{
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
