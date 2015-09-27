/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
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

// Project imports -------------------------
import "qrc:/declarative-camera"
import "qrc:/pages-upload"

Item {

    // Pages -------------------------------
    Component{
        id: page_TakeCameraPicture

        BasicPage{
            title: qsTr('Camera')

            CameraUi{
                anchors.fill: parent

                onImageSaved: {
                    wa_PictureUploader.setCameraPictureFilePath(path);
                    stackView.push(page_LocationCheck);
                    navigator.backButtonVisible     = true;
                    navigator.continueButtonVisible = false;
                    navigator.menuButtonVisible     = false;
                    navigator.title = page_LocationCheck.title;
                }
            }
        }
    }

    CropPicture{
        id: page_CropPicture

        onCropFinished: {
            wa_PictureUploader.setCropPicture(source,
                                              cropX,
                                              cropY,
                                              cropSide);
            stackView.push(page_LocationCheck);
            navigator.backButtonVisible     = true;
            navigator.continueButtonVisible = false;
            navigator.menuButtonVisible     = false;
            navigator.title = page_LocationCheck.title;
        }
    }

    LocationCheck{
       id: page_LocationCheck

       onLocationAccepted: {
           stackView.push(page_NearbySpotSelection);
           navigator.backButtonVisible     = true;
           navigator.continueButtonVisible = false;
           navigator.menuButtonVisible     = false;
           navigator.title = page_NearbySpotSelection.title;
       }
    }

    NearbySpotSelection{
        id: page_NearbySpotSelection
        visible: false

        onAddNewSpot: {
            stackView.push(page_AddNewSpot)
            navigator.backButtonVisible     = true;
            navigator.continueButtonVisible = false;
            navigator.menuButtonVisible     = false;
            navigator.title = page_AddNewSpot.title;
        }

        onAddToExistingSpot: {
            stackView.push(page_AddToExistingSpot);
            navigator.backButtonVisible     = true;
            navigator.continueButtonVisible = false;
            navigator.menuButtonVisible     = false;
            navigator.title = page_AddToExistingSpot.title;
        }
    }

    AddNewSpot{
        id: page_AddNewSpot
        visible: false

        onUploadSuccessfull: {
            stackView.pop(page_SourceSelection);
            tabWidget_Main.setCurrentItem(panel_Home);
        }
    }

    Page_AddToExistingSpot{
        id: page_AddToExistingSpot
        visible: false

        onUploadSuccessfull: {
            stackView.pop(page_SourceSelection);
            tabWidget_Main.setCurrentItem(panel_Home);
        }
    }

    // Gui ---------------------------------
    Navigator{
        id: navigator
        anchors.top: parent.top

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
        anchors.top: navigator.bottom
        anchors.bottom: parent.bottom
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true;
                         }
        initialItem: SourceSelection {
            id: page_SourceSelection
            width: parent.width
            height: parent.height

            onTakeCameraPicture: {
                stackView.push(page_TakeCameraPicture)
                navigator.backButtonVisible     = true;
                navigator.continueButtonVisible = false;
                navigator.menuButtonVisible     = false;
                navigator.title = page_TakeCameraPicture.title;
            }
            onPictureSelected: {
                page_CropPicture.source = imageUrl;
                stackView.push(page_CropPicture);
                navigator.backButtonVisible     = true;
                navigator.continueButtonVisible = false;
                navigator.menuButtonVisible     = false;
                navigator.title = page_CropPicture.title;
            }
        }

        onDepthChanged: {
            if(depth === 1)
            {
                navigator.backButtonVisible     = false;
                navigator.continueButtonVisible = false;
                navigator.menuButtonVisible     = false;
                navigator.title = page_SourceSelection.title;
            }
        }
    }


    // Slots -------------------------------
    onVisibleChanged: {
        if(visible == false)
            return;

        if(wa_User.isConnected() === false)
        {
            stackView.push( {item: Qt.resolvedUrl("qrc:/pages-user/Page_SignIn.qml"),
                             immediate: true,
                             replace: false,
                             properties:{width:stackView.width,
                                         height:stackView.height,
                                         stackView:stackView,
                                         navigator:navigator}} );

            return;
        }
    }
}
