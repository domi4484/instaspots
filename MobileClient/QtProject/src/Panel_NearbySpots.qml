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
import "qrc:/widgets"

Item {

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

            wa_NearbySpotModel.setLocation(hc_LocationManager.latitude(),
                                           hc_LocationManager.longitude());
        }
    }

    // Slots -------------------------------
    onVisibleChanged: {
        if(visible == false)
            return;

        // Set current location (also if outdated)
        wa_NearbySpotModel.setLocation(hc_LocationManager.latitude(),
                                       hc_LocationManager.longitude());

        // Request location update
        hc_LocationManager.requestLocation();

        // TODO if current location not valid hc_LocationManager.isValid then display waiting animation
    }

    // Components --------------------------
    Component {
        id: component_SpotOverview

        Item {
            id: root
            width: parent.width
            height:   rectangle_Top.height
                      + image_Picture1.height

            signal spotClicked

            Rectangle {
                id: rectangle_Top
                anchors.top: parent.top
                width: parent.width
                height: 40
                color: "#aaaaaa"
                Text{
                    id: text_SpotName
                    anchors.leftMargin: 2
                    width: parent.width / 2
                    height: parent.height / 2
                    text: role_SpotName
                }
                Text {
                    anchors.top: parent.top
                    anchors.left: text_SpotName.right
                    anchors.rightMargin: 2
                    width: text_SpotName.width
                    height: text_SpotName.height
                    horizontalAlignment: Text.AlignRight
                    text: role_SpotDistance
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        spotClicked();
                    }
                }
            }
            CachedPicture {
                id: image_Picture1
                anchors.top: rectangle_Top.bottom
                width: parent.width/2
                height: parent.width/2
                sourceUrl: role_SpotPictureUrl1
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        spotClicked();
                    }
                }
            }
            CachedPicture {
                id: image_Picture2
                anchors.top: rectangle_Top.bottom
                anchors.left: image_Picture1.right
                width: parent.width/2
                height: parent.width/2
                sourceUrl: role_SpotPictureUrl2
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        spotClicked();
                    }
                }
            }
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
                id: listView_SpotsOverview
                width: parent.width
                height: parent.height
                model: wa_NearbySpotModel
                delegate: component_SpotOverview
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
