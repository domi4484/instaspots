/********************************************************************
 *                                                                 *
 * Lowespot                                                        *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      02.09.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.3

// Project c++ imports ---------------------
import Picture       1.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"
import "qrc:/qml/pages-picture"
import "qrc:/qml/component"

Item{
    id: page_Likes


    // Properties --------------------------

    property int       pictureId: -1
    property Picture   picture


    // Navigation properties ---------------

    property string navigation_Title:                 ""
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Gui ---------------------------------

    ListView {
        id: listView_Likes

        anchors.fill: parent

        model: picture.likers

        delegate: Link {
            text: modelData.username

            height: 30 *hc_Application.dip

            verticalAlignment: Text.AlignVCenter

            onClicked: {
                stackView.push( { item: Qt.resolvedUrl("qrc:/qml/pages-user/Page_User.qml"),
                                  properties : {
                                                 navigation_Title : modelData.username,
                                                 stackView        : stackView,
                                                 userId           : modelData.id
                                                } });
            }
        }
    } // Listview


    // Signals -----------------------------

    onPictureIdChanged:
    {
        picture = re_PictureRepository.getBy_PictureId(pictureId);
    }
}
