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
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

// Project qml imports ---------------------
import "qrc:/qml/"

Item{
    id: addNewSpot


    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("New spot")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Message boxes -----------------------

    MessageDialog{
        id: messageDialog_Error

        title: qsTr('Upload error')
    }

    MessageDialog{
        id: messageDialog_UploadSuccessful

        title: qsTr('Upload successfull')
        text:  qsTr('Spot succesfully uploaded')
    }

    MessageDialog {
        id: messageDialog_Help

        title: qsTr("Secret spot")
        text:  qsTr("The location of a secret spot is not displayed to other users, until they upload a picture to the spot.")

        onAccepted: hc_Settings.set_HelpGotIt_UploadNewPictureSecretSpot(true)
    }

    // Connections -------------------------

    Connections{
        target: wa_PictureUploader
        onSignal_UploadFinished:
        {
            if(wa_PictureUploader.isNewSpot() === false)
            {
                return;
            }

            button_UploadNewSpot.enabled = true;

            if(success == false)
            {
                messageDialog_Error.text = wa_PictureUploader.lastErrorText();
                messageDialog_Error.visible = true;
                return;
            }

            messageDialog_UploadSuccessful.visible = true;

            stackView.pop(page_SourceSelection);
            tabWidget_Main.setCurrentItem(panel_Home);
            panel_Home.tabWidget_CurrentTabReclicked();

            wa_PictureUploader.resetDefaults()
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

        // Spot name
        Text{
            text: qsTr("Spot name")
        }

        TextField{
            id: textField_SpotName
            width: parent.width
        }

        // Secret spot
        Item{
            anchors.left:  parent.left
            anchors.right: parent.right

            height: switch_SecretSpot.height

            Text{
                id: text_SecretSpot

                anchors.left: parent.left

                text: qsTr("Secret spot")
            }

            ToolButton{
                anchors.left:           text_SecretSpot.right
                anchors.verticalCenter: text_SecretSpot.verticalCenter
                height: parent.height
                width : height

                visible: hc_Settings.get_HelpGotIt_UploadNewPictureSecretSpot() === false

                Image {
                    anchors.fill:    parent
                    anchors.margins: 2

                    smooth: true
                    source: "qrc:/icon/icon/dialog-question.png"
                }

                onClicked: messageDialog_Help.open()
            }

            Switch{
                id: switch_SecretSpot

                anchors.right:          parent.right
                anchors.verticalCenter: text_SecretSpot.verticalCenter

                checked: false
                onCheckedChanged: wa_PictureUploader.setSecretSpot(checked)
            }
        }

        // Spot description
        Text{
            text: qsTr("Spot description")
        }
    }
    TextArea{
        id: textArea_Description

        width: parent.width / 1.1
        anchors.top:    column.bottom
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
                return;
            }

            enabled = false;
        }
    }
}

