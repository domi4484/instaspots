import QtQuick 2.2

Rectangle {
    width: 100
    height: 62

    Text {
        text: qsTr("Starting up...")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Component.onCompleted: {
        wa_User.login();
    }
}
