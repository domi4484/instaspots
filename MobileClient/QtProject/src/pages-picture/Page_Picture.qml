/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      23.10.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.3

// Project c++ imports ---------------------
import PicturesModel 1.0

// Project qml imports ---------------------
import "qrc:/"
import "qrc:/widgets"
import "qrc:/pages-user"

Item{
    id: page_Picture


    // Properties --------------------------

    property Navigator navigator
    property StackView stackView


    // Bind properties ---------------------

    property alias model: picturesModel


    // Navigation properties ---------------

    property string navigation_Title:                 picturesModel.first().role_SpotName
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Models ------------------------------

    PicturesModel{
        id: picturesModel
    }


    // Gui ---------------------------------

    ListView {
        id: listView
        anchors.fill: parent
        model: picturesModel
        delegate: SpotViewDelegate{

            onUserClicked: {
                stackView.push( {
                                   item       : Qt.resolvedUrl("qrc:/pages-user/Page_User.qml"),
                                   properties : {
                                                  navigation_Title : role_UserUsername,
                                                  stackView        : stackView,
                                                  userId           : role_UserId
                                                }
                                } );
            }
            onSpotClicked: {
                stackView.push( {
                                   item       : Qt.resolvedUrl("qrc:/pages-spot/Page_Spot.qml"),
                                   properties : {
                                                  stackView        : stackView,
                                                  spotId           : role_SpotId
                                                }
                                } );
            }
        }
    }
}
