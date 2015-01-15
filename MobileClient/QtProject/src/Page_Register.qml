import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {

    property string title: qsTr("Register")

    Column{
        anchors.fill: parent
        anchors.topMargin: 5
        spacing: 5

        TextField {
            id: textField_Username
            placeholderText: qsTr("Username")
            width: parent.width / 1.3
            anchors.horizontalCenter: parent.horizontalCenter

            onTextChanged: checkUsername(text)
        }
        TextField {
            id: textField_EMail
            placeholderText: qsTr("e Mail")
            width: parent.width / 1.3
            anchors.horizontalCenter: parent.horizontalCenter
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

            onClicked: wa_User.register(textField_Username.text,
                                        textField_EMail.text,
                                        textField_Password.text)
        }
    }

    function checkUsername(username)
    {
        if(username.length > 0)
        {
            wa_User.checkUsername(username);
        }
        else
        {
            // todo nascondere il testo "Username non disponibile"
        }
    }
}

