
// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.2

// Project imports -------------------------
import "qrc:/"
import "qrc:/pages-picture/"
import "qrc:/pages-spot/"

Rectangle {

    // Signals -----------------------------
    Component.onCompleted:
    {
        wa_NewsModel.getNewestSpots();
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

        initialItem: Page_PicturesList {
            width: parent.width
            height: parent.height
            title: qsTr("News")
            model: wa_NewsModel

            onUserClicked: {
                console.log(username);
            }

            onSpotClicked: {
                stackView.push(Qt.resolvedUrl("pages-spot/Page_Spot.qml"));
                stackView.currentItem.spotId = spotId;

                navigator.title = spotName;

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
