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
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

// Project imports -------------------------
import "qrc:/"

BasicPage{
    id: addNewSpot

    // BasicPage properties ----------------
    title: qsTr("New spot")
    continueButtonVisible: false

    // Signals -----------------------------
    signal uploadSuccessfull()

    // Message boxes -----------------------
    MessageDialog{
        id: messageDialog_Error
        title: qsTr('Upload error')
    }

    MessageDialog{
        id: messageDialog_UploadSuccessful
        title: qsTr('Upload successfull')
        text: qsTr('Spot succesfully uploaded')
    }

    // Connections -------------------------
    Connections{
        target: wa_PictureUploader
        onSignal_UploadFinished:{
            if(success == false)
            {
                messageDialog_Error.text = wa_PictureUploader.lastErrorText();
                messageDialog_Error.visible = true;
                return;
            }

            messageDialog_UploadSuccessful.visible = true;
            uploadSuccessfull();
        }
    }

    // Gui ---------------------------------
    Column{
        id: column
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 5
        width: parent.width / 1.1
        spacing: 5

        Text{
            text: qsTr("Spot name")
        }

        TextField{
            id: textField_SpotName
            width: parent.width
        }

        Text{
            text: qsTr("Spot description")
        }
    }
    TextArea{
        id: textArea_Description
        width: parent.width / 1.1
        anchors.top: column.bottom
        anchors.bottom: button_UploadNewSpot.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button{
        id: button_UploadNewSpot
        width: parent.width / 1.1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("Upload new spot")

        onClicked: {
            wa_PictureUploader.setName(textField_SpotName.text);
            wa_PictureUploader.setDescription(textArea_Description.text);
            if(wa_PictureUploader.execute() === false)
            {
                messageDialog_Error.text = wa_PictureUploader.lastErrorText();
                messageDialog_Error.visible = true;
            }
        }
    }
}

