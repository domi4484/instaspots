/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      01.04.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

// Project imports -------------------------
import "qrc:/"

BasicPage{
    id: page_addToExistingSpot

    // BasicPage properties ----------------
    title: qsTr("Add picture to spot")
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
        text: qsTr('Picture succesfully uploaded')
    }

    // Connections -------------------------
    Connections{
        target: wa_PictureUploader
        onSignal_UploadFinished:
        {
            button_UploadPicture.enabled = true;

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

    onVisibleChanged: {
        if(visible)
        {
            text_SpotName.text = wa_PictureUploader.name();
            text_Description.text = wa_PictureUploader.description();
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
            text: qsTr("Spot name:")
        }

        Text{
            id: text_SpotName
            width: parent.width
        }

        Text{
            text: qsTr("Spot description:")
        }
    }
    Text{
        id: text_Description
        width: parent.width / 1.1
        anchors.top: column.bottom
        anchors.bottom: button_UploadPicture.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button{
        id: button_UploadPicture
        width: parent.width / 1.1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("Upload picture")

        onClicked: {
            if(wa_PictureUploader.execute() === false)
            {
                messageDialog_Error.text = wa_PictureUploader.lastErrorText();
                messageDialog_Error.visible = true;
                return;
            }

            enabled = false;
        }
    }
}

