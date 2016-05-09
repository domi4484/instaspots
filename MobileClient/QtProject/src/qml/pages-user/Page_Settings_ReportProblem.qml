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

    property string navigation_Title:                 qsTr("Report a problem")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Message boxes -----------------------

    MessageDialog{
        id: messageDialog

        title: qsTr('Report problem')
    }


    // Gui ---------------------------------

    // Description
    Text{
        id: text_ProblemDescription

        anchors.margins: 5
        anchors.top:   parent.top
        anchors.left:  parent.left
        anchors.right: parent.right

        text: qsTr("Problem description:")
    }

    TextArea{
        id: textArea_ProblemDescription

        anchors.margins: 5
        anchors.top:    text_ProblemDescription.bottom
        anchors.bottom: item_AttachTraces.top
        anchors.left:   parent.left
        anchors.right:  parent.right
    }

    // Attach traces
    Item{
        id: item_AttachTraces

        anchors.margins: 5
        anchors.bottom: button_SendReport.top
        anchors.left:   parent.left
        anchors.right:  parent.right

        height: switch_AttachTraces.height

        Text{
            id: text_SecretSpot

                anchors.left: parent.left

                text: qsTr("Attach traces")
            }

            Switch{
                id: switch_AttachTraces

                anchors.right:          parent.right
                anchors.verticalCenter: text_SecretSpot.verticalCenter

                checked: true
                onCheckedChanged: wa_PictureUploader.setSecretSpot(checked)
            }
    }

    Button{
        id: button_SendReport

        anchors.margins: 5
        anchors.bottom: parent.bottom
        anchors.left:   parent.left
        anchors.right:  parent.right

        text: qsTr("Send report")

        onClicked: {
            if(hc_Application.reportProblem(textArea_ProblemDescription.text,
                                            switch_AttachTraces.checked)
                    === false)
            {
                messageDialog.title   = qsTr("Report error");
                messageDialog.text    = hc_Application.lastErrorText();
                messageDialog.visible = true;
                return;
            }

            messageDialog.title   = qsTr("Problem reported");
            messageDialog.text    = qsTr("Thank you for reporting.");
            messageDialog.visible = true;
        }
    }
}

