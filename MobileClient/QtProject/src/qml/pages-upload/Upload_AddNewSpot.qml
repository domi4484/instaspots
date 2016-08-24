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
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/component/"

Item{
    id: upload_AddNewSpot


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

    MessageDialog {
        id: messageDialog_Help

        title: qsTr("Secret spot")
        text:  qsTr("The location of a secret spot is not displayed to other users, until they upload a picture to the spot.")
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

            // Change view to Home
            stackView.pop(page_Upload_SourceSelection);
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
        Component_TagSwitch{
            anchors.left:  parent.left
            anchors.right: parent.right

            text: qsTr("Secret spot")
            helpButtonVisible: true

            onCheckedChanged:   wa_PictureUploader.setSecretSpot(checked)
            onHelpButtonClicked: messageDialog_Help.open()
        }

        // Skatepark
        Component_TagSwitch{
            id: component_TagSwitch_Skatepark

            anchors.left:  parent.left
            anchors.right: parent.right

            text: qsTr("Skatepark")
        }

        // Street
        Component_TagSwitch{
            id: component_TagSwitch_Street

            anchors.left:  parent.left
            anchors.right: parent.right

            text: qsTr("Street")
        }

        // Miniramp
        Component_TagSwitch{
            id: component_TagSwitch_Miniramp

            anchors.left:  parent.left
            anchors.right: parent.right

            text: qsTr("Miniramp")
        }

        // Bowl
        Component_TagSwitch{
            id: component_TagSwitch_Bowl

            anchors.left:  parent.left
            anchors.right: parent.right

            text: qsTr("Bowl")
        }

        // Transition: Categorie Bowl, miniramp, vert, microramp, quarter, wallride
        // Categoria rail Handrail, flatrail
        // Ledge curb
        // Stairs
        // Bank

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
            wa_PictureUploader.setDescription("");

            // Tags
            if(component_TagSwitch_Skatepark.checked)
            {
                wa_PictureUploader.addTag("skatepark");
            }

            if(component_TagSwitch_Street.checked)
            {
                wa_PictureUploader.addTag("street");
            }

            if(component_TagSwitch_Miniramp.checked)
            {
                wa_PictureUploader.addTag("miniramp");
            }

            if(component_TagSwitch_Bowl.checked)
            {
                wa_PictureUploader.addTag("bowl");
            }

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

