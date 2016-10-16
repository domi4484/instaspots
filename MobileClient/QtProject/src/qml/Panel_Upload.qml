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

// Project c++ imports ---------------------
import SpotsModel 1.0

// Project qml imports ---------------------
import "qrc:/qml/declarative-camera"
import "qrc:/qml/pages-upload"

Item {

    SpotsModel{
        id: spotsModel_NearbySpotSelection
    }


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
                    stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-upload/Upload_CropPicture.qml"),
                                    properties:{stackView        : stackView,
                                                source           : path}});
                }
            }
        }
    }


    // Gui ---------------------------------

    Navigator{
        id: navigator

        anchors.top: parent.top
        height:      30 * hc_Application.dip
        width:       parent.width

        title                 : (stackView.currentItem != null) ? stackView.currentItem.navigation_Title                 : "";
        backButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_BackButtonVisible     : false;
        continueButtonVisible : (stackView.currentItem != null) ? stackView.currentItem.navigation_ContinueButtonVisible : false;
        menuButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_MenuButtonVisible     : false;

        customButtonLeftSource  : (stackView.currentItem != null) ? (stackView.currentItem.navigator_customButtonLeftSource != null)  ? stackView.currentItem.navigator_customButtonLeftSource  : "" : "";
        customButtonRightSource : (stackView.currentItem != null) ? (stackView.currentItem.navigator_customButtonRightSource != null) ? stackView.currentItem.navigator_customButtonRightSource : "" : "";

        onPreviousPage: {
            if(stackView.depth > 1)
                stackView.pop();
        }

        onContinueClicked: {
            if(stackView.currentItem == null)
                return;

            if(stackView.currentItem.navigator_ContinueButtonClicked != null)
                stackView.currentItem.navigator_ContinueButtonClicked();
        }

        onMenuClicked: {
            if(stackView.currentItem == null)
                return;

            if(stackView.currentItem.navigator_MenuButtonClicked != null)
                stackView.currentItem.navigator_MenuButtonClicked();
        }

        onCustomButtonLeftClicked: {
            if(stackView.currentItem == null)
                return;

            if(stackView.currentItem.navigator_CustomButtonLeftClicked != null)
                stackView.currentItem.navigator_CustomButtonLeftClicked();
        }

        onCustomButtonRightClicked: {
            if(stackView.currentItem == null)
                return;

            if(stackView.currentItem.navigator_CustomButtonRightClicked != null)
                stackView.currentItem.navigator_CustomButtonRightClicked();
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

        initialItem: Upload_SourceSelection {
            id: page_Upload_SourceSelection
            width : parent.width
            height: parent.height

            onTakeCameraPicture: {
                stackView.push(page_TakeCameraPicture)
            }
            onPictureSelected: {
                stackView.push({item: Qt.resolvedUrl("qrc:/qml/pages-upload/Upload_CropPicture.qml"),
                                properties:{stackView        : stackView,
                                            source           : imageUrl}});
            }
        }
    }


    // Slots -------------------------------

    onVisibleChanged: {
        if(visible == false)
        {
            hc_LocationManager.stopUdates();
            return;
        }

        // Is user logged in?
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

        // Set current location (also if outdated)
        spotsModel_NearbySpotSelection.getBy_Distance(hc_LocationManager.coordinate,
                                                      2 * 1000); // 2km

        // Request location update
        hc_LocationManager.startUpdates();
    }
}
