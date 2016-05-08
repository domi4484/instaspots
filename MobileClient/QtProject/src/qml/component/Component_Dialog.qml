import QtQuick 2.5
import QtQuick.Dialogs 1.2

MessageDialog {
    id: messageDialog_NewClientVersionAvailable
    title: qsTr('New version out!')
    text: qsTr('There is a new Lowerspot app version available for download! Get it on lowerspot.com!')

    onAccepted:
    {
        hc_Application.newerClientVersionAvailableGotIt();
    }
}
