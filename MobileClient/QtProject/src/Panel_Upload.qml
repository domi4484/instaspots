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
        }
    }

    LocationCheck{
       id: page_LocationCheck

       onLocationAccepted: {
           stackView.push(page_NearbySpotSelection);
       }
    }

    NearbySpotSelection{
        id: page_NearbySpotSelection
        visible: false

        onAddNewSpot: {
            stackView.push(page_AddNewSpot)
        }
    }

    AddNewSpot{
        id: page_AddNewSpot
        visible: false

        onUploadSuccessfull: {
            stackView.pop(page_SourceSelection);
        }
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

            onTakeCameraPicture: stackView.push(page_TakeCameraPicture)
            onPictureSelected: {
                page_CropPicture.source = imageUrl;
                stackView.push(page_CropPicture);
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
