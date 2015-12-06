/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      14.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.2

// Project c++ imports ---------------------

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/pages-picture/"
import "qrc:/pages-spot/"

Item {

    // Gui ---------------------------------

    Navigator{
        id: navigator

        anchors.left:  parent.left
        anchors.right: parent.right
        anchors.top:   parent.top

        title                 : (stackView.currentItem != null) ? stackView.currentItem.navigation_Title                 : "";
        backButtonVisible     : stackView.depth > 1
        continueButtonVisible : (stackView.currentItem != null) ? stackView.currentItem.navigation_ContinueButtonVisible : false;
        menuButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_MenuButtonVisible     : false;

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

        initialItem: Page_PicturesList {
            id: page_PicturesList
            width : parent.width
            height: parent.height

            navigation_Title: qsTr("News")

            Component.onCompleted: {
                model.getNewestSpots();
            }

            onUserClicked: {
                stackView.push({item: Qt.resolvedUrl("qrc:/pages-user/Page_User.qml"),
                               properties:{navigation_Title : username,
                                           stackView        : stackView,
                                           userId           : userId}});
            }

            onSpotClicked: {
                stackView.push({item: Qt.resolvedUrl("qrc:/pages-spot/Page_Spot.qml"),
                               properties:{navigation_Title : spotName,
                                           stackView        : stackView,
                                           spotId           : spotId }});
            }
        }
    }
}
