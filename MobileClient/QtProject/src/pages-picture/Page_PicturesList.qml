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

// Project imports -------------------------
import "qrc:/"
import "qrc:/views"

BasicPage{
    id: page_PicturesList

    // BasicPage properties ----------------
    title: qsTr("Pictures")
    continueButtonVisible: false
    menuButtonVisible: false

    // Bind properties ---------------------
    property alias model: listView.model

    // Signals -----------------------------
    signal userClicked(int userId, string username)
    signal spotClicked(int spotId, string spotName)

    // Gui ---------------------------------
    ListView {
        id: listView
        anchors.fill: parent
        model: wa_NewsModel
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

