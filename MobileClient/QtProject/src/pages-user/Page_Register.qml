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

// Project qml imports ---------------------
import "qrc:/"

Item {

    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Register")
    property bool   navigation_BackButtonVisible:     false
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Message boxes -----------------------

    MessageDialog{
        id: messageDialog
        title: qsTr('Registration error')
    }


    // Connections -------------------------

    Connections {
        target: wa_User
        onSignal_RegistrationSuccessfull:
        {
            button_Register.enabled = true;

            // Register successfull
            if(success == false)
            {
                messageDialog.text = wa_User.lastErrorText();
                messageDialog.visible = true;
                return;
            }
        }
    }


    // Gui ---------------------------------

    Column{
        anchors.fill: parent
        anchors.topMargin: 5
        spacing: 5

        TextField {
            id: textField_Username
            placeholderText: qsTr("Username")
            width: parent.width / 1.3
            anchors.horizontalCenter: parent.horizontalCenter

            inputMethodHints: Qt.ImhNoAutoUppercase
            validator: RegExpValidator {regExp: /[a-z1-9\_\.]+/;}
        }
        TextField {
            id: textField_EMail
            placeholderText: qsTr("e Mail")
            width: parent.width / 1.3
            anchors.horizontalCenter: parent.horizontalCenter

            validator: RegExpValidator { regExp:/\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/ }
        }
        TextField {
            id: textField_Password
            echoMode: TextInput.Password
            placeholderText: qsTr("Password")
            width: parent.width / 1.3
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: button_Register
            text: qsTr("Register")
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked:
            {
                if(wa_User.registration(textField_Username.text,
                                        textField_EMail.text,
                                        textField_Password.text) === false)
                {
                    messageDialog.text = wa_User.lastErrorText();
                    messageDialog.visible = true;
                    return;
                }

                button_Register.enabled = false;
            }
        }
    }
}

