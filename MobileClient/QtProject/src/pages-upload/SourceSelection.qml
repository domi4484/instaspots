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
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4

// Project imports -------------------------
import "qrc:/"

BasicPage{

    // BasicPage properties ----------------
    title: qsTr("Upload")
    continueButtonVisible: false

    // Signals -----------------------------
    signal takeCameraPicture()
    signal pictureSelected(string imageUrl)

    Component.onCompleted: {
        if(QtMultimedia.availableCameras.length <= 0)
        {
            button_Camera.enabled = false
        }
    }

    // Gui ---------------------------------
    Button {
        id: button_Camera
        anchors.top: parent.top
        height: parent.height/2
        width: parent.width
        text: qsTr("Camera")

        onClicked: {
            takeCameraPicture();
        }
    }

    Button {
        id: button_Gallery
        anchors.top: button_Camera.bottom
        height: parent.height/2
        width: parent.width
        text: qsTr("Gallery")

        FileDialog {
            id: fileDialog
            title: "Please choose a picture"
            nameFilters: [ "Image files (*.jpg *.JPG *.jpeg *.JPEG *.png *.PNG)" ]

            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrl);

                pictureSelected(fileDialog.fileUrl);
            }
        }

        onClicked: {
            fileDialog.visible = true;
        }
    }
}

