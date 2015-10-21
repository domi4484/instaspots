/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      31.03.2015                                        *
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
import "qrc:/views"

Item{
    id: page_PicturesList


    // Bind properties ---------------------

    property alias model: picturesModel


    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Pictures")
    property bool   navigation_BackButtonVisible:     false
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Model -------------------------------

    PicturesModel{
        id: picturesModel
    }


    // Signals -----------------------------

    signal userClicked(int userId, string username)
    signal spotClicked(int spotId, string spotName)


    // Gui ---------------------------------

    ListView {
        id: listView
        anchors.fill: parent
        model: picturesModel
        delegate: SpotViewDelegate{

            onUserClicked: {
                page_PicturesList.userClicked(role_UserId, role_UserUsername);
            }
            onSpotClicked: {
                page_PicturesList.spotClicked(role_SpotId, role_SpotName);
            }
        }
    }
}

