
// Qt imports ------------------------------
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2

// Project imports -------------------------
import "qrc:/qml/"

Rectangle {
    id: page_Login

    // Properties --------------------------
    MessageDialog{
        id: messageDialog
        title: qsTr('Login error')
    }

    // Connections -------------------------
    Connections {
        target: wa_User
        onSignal_LoginSuccessfull:
        {

            button_Login.enabled = true;

            // Login failed
            if(success == false)
            {
                messageDialog.text = wa_User.lastErrorText();
                messageDialog.visible = true;
                return;
            }
        }
    }


    // Gui ---------------------------------
    width: parent.width
    height: parent.height

    TextField {
        id: textField_Username
        placeholderText: qsTr("Username")
        width: parent.width / 1.3
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        inputMethodHints: Qt.ImhNoAutoUppercase
        validator: RegExpValidator {regExp: /[a-z1-9\_\.]+/;}

        Keys.onEnterPressed:
        {
            login(textField_Username.text,
                  textField_Password.text)
        }
    }
    TextField {
        id: textField_Password
        echoMode: TextInput.Password
        placeholderText: qsTr("Password")
        width: parent.width / 1.3
        anchors.top: textField_Username.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        Keys.onEnterPressed:
        {
            login(textField_Username.text,
                  textField_Password.text)
        }
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

    // Functions ---------------------------
    function login(username,
                   password)
    {
        if(wa_User.login(username,
                         password) === false)
        {
            messageDialog.text = wa_User.lastErrorText();
            messageDialog.visible = true;
            return;
        }

        button_Login.enabled = false;
    }
}





