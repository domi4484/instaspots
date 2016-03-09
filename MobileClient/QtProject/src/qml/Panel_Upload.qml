/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      30.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4

// Project qml imports ---------------------
import "qrc:/qml/declarative-camera"
import "qrc:/qml/pages-upload"

Item {


    // Pages -------------------------------

    Component{
        id: page_TakeCameraPicture

        Item{

            // Navigation properties ---------------

            property string navigation_Title:                 qsTr('Camera')
            property bool   navigation_BackButtonVisible:     true
            property bool   navigation_ContinueButtonVisible: false
            property bool   navigation_MenuButtonVisible:     false

            CameraUi{
                anchors.fill: parent

                onImageSaved: {
                    wa_PictureUploader.setCameraPictureFilePath(path);
                    stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-upload/LocationCheck.qml"),
                                    properties:{stackView        : stackView}});
                }
            }
        }
    }


    // Gui ---------------------------------

    Navigator{
        id: navigator

        anchors.top: parent.top
        width:       parent.width
        height:      40

        title                 : (stackView.currentItem != null) ? stackView.currentItem.navigation_Title                 : "";
        backButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_BackButtonVisible     : false;
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

        initialItem: SourceSelection {
            id: page_SourceSelection
            width : parent.width
            height: parent.height

            onTakeCameraPicture: {
                stackView.push(page_TakeCameraPicture)
            }
            onPictureSelected: {
                stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-upload/CropPicture.qml"),
                                properties:{stackView        : stackView,
                                            source           : imageUrl}});
            }
        }
    }


    // Slots -------------------------------

    onVisibleChanged: {
        if(visible == false)
            return;

        if(wa_User.isConnected() === false)
        {
            var page_SignIn = Qt.resolvedUrl("qrc:/qml/pages-user/Page_SignIn.qml");
            stackView.push( {
                               item       : page_SignIn,
                               immediate  : true,
                               replace    : false,
                               properties : { stackView : stackView }
                             } );
            return;
        }
    }
}
