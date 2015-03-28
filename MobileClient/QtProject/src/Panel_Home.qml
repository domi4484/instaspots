
// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {

    Component.onCompleted:
    {
        wa_NewsModel.getNewestSpots();
    }

    Navigator{
        id: navigator
        anchors.top: parent.top

        title: qsTr("News")
        backButtonVisible: false
    }

    StackView {
        id: stackView
        anchors.top: navigator.bottom
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true;
                         }

        initialItem: Item {
            width: parent.width
            height: parent.height
            ListView {
                anchors.fill: parent
                model: wa_NewsModel
                delegate: SpotViewDelegate{

                    onUserClicked: {
                        console.log(username)
                    }

                    onSpotClicked: {
                        console.log(idSpot)
                    }
                }
            }
        }
    }
}
