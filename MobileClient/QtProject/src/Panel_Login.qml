import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2

Rectangle {
    // Gui ---------------------------------
    Navigator{
        id: navigator
        anchors.top: parent.top

        title: qsTr("Login")
        backButtonVisible: stackView.depth > 1

        onPreviousPage: {
            if(stackView.depth > 1)
                stackView.pop();
        }

        Connections{
            target: stackView
            onDepthChanged: {
                if(stackView.depth == 1)
                    navigator.title = qsTr("Login")
            }
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
        initialItem: Item {
            width: parent.width
            height: parent.height

            TextField {
                id: textField_Username
                placeholderText: qsTr("Username")
                width: parent.width / 1.3
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }
            TextField {
                id: textField_Password
                echoMode: TextInput.Password
                placeholderText: qsTr("Password")
                width: parent.width / 1.3
                anchors.top: textField_Username.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Button {
                id: button_Login
                text: qsTr("Login")
                anchors.top: textField_Password.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: login(textField_Username.text,
                                 textField_Password.text)
            }

            Text {
                id: text_Or
                text: qsTr("or")
                anchors.top: button_Login.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: button_GoRegister
                text: qsTr("Register")
                anchors.top: text_Or.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    var page_Register = stackView.push( Qt.createComponent("Page_Register.qml") );
                    navigator.title = page_Register.title;
                }
            }
        }
    }
    // Gui ---------------------------------

    MessageDialog{
        id: messageDialog
        title: qsTr('Login error')
    }

    function login(username,
                   password)
    {
        if(username.length === 0)
        {
            messageDialog.text = qsTr("Username is empty.")
            messageDialog.visible = true;
            return;
        }

        if(password.length === 0)
        {
            messageDialog.text = qsTr("Password is empty.")
            messageDialog.visible = true;
            return;
        }

        button_Login.enabled = false;
        wa_User.login(username,
                      password)
    }

    Connections {
        target: wa_User
        onSignal_LoginSuccessfull: {

            button_Login.enabled = true;

            // Login successfull
            if(success == false)
            {
                messageDialog.text = qsTr("Authentication failed.")
                messageDialog.visible = true;
                return;
            }
        }
    }
}





