/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      26.08.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.4
import QtQuick.Controls 1.3

// Project c++ imports ---------------------
import SpotsModel 1.0
import PicturesModel 1.0

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/widgets"
import "qrc:/views"
import "qrc:/pages-spot"

BasicPage{
    id: page_User

    // BasicPage properties ----------------
    continueButtonVisible: false
    menuButtonVisible: false

    // Properties --------------------------
    property int userId: -1
    property Navigator navigator
    property StackView stackView

    PicturesModel{
        id: picturesModel
    }

    SpotsModel{
        id: spotsModel
    }

    // Gui ---------------------------------
    TabWidgetTop {
        id: tabWidget
        width: parent.width
        height: parent.height

        Item{
            id: tab_Pictures
            anchors.fill: parent
            property string tabWidget_ButtonText: qsTr("Pictures(%1)").arg(picturesModel.count)
            property string tabWidget_ButtonIconSource: ""

            GridView{
                id: gridView_Pictures
                anchors.fill: parent

                cellWidth: parent.width / 2
                cellHeight: cellWidth

                model: picturesModel
                delegate: component_Picture
            }
        }

        StackView {
            id: stackView
            anchors.fill: parent

            // Tab widget properties
            property string tabWidget_ButtonText: qsTr("Spots(%1)").arg(spotsModel.count)
            property string tabWidget_ButtonIconSource: ""

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
                    anchors.fill: parent
                    model: spotsModel
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
        }
    }



    // Signals -----------------------------
    onUserIdChanged:
    {
        picturesModel.setUserId(userId);
        spotsModel.setUserId(userId);
    }

    // Connections -------------------------

    // Components --------------------------
    Component {
        id: component_Picture

        Item {
            signal pictureClicked

            width: gridView_Pictures.cellWidth
            height: gridView_Pictures.cellHeight

            CachedPicture {
                id: image_Picture
                anchors.fill: parent
                sourceUrl: role_PictureUrl
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        stackView.push({item: Qt.resolvedUrl("qrc:/pages-spot/Page_Spot.qml"),
                                        properties:{width:stackView.width,
                                                    height:stackView.height,
                                                    stackView:stackView,
                                                    navigator:navigator,
                                                    spotId:role_SpotId}});
                        navigator.backButtonVisible = true;
                    }
                }
            }
        }
    }
}
