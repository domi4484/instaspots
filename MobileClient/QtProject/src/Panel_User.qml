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
import QtQuick 2.2
import QtQuick.Controls 1.1

// Project imports -------------------------
import "qrc:/views"
import "qrc:/pages-user"

Rectangle {

    // Pages -------------------------------
    Page_Settings {
        id: page_Settings
        visible: false
    }

    // Slots -------------------------------
    onVisibleChanged: {
        if(visible == false)
            return;


        if(wa_User.isConnected() === false)
        {
            stackView.pop();
            var page_SignIn = Qt.resolvedUrl("qrc:/pages-user/Page_SignIn.qml");
            stackView.push( {item: page_SignIn,
                             immediate: true,
                             replace: true,
                             properties:{width:stackView.width,
                                         height:stackView.height,
                                         stackView:stackView,
                                         navigator:navigator}} );


        }
    }

    // Gui ---------------------------------
    Navigator{
        id: navigator
        anchors.top: parent.top
        backButtonVisible: stackView.depth > 1

        onPreviousPage: {
            if(stackView.depth > 1)
                stackView.pop();
        }

        onMenuClicked: {
            stackView.push(page_Settings);
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
        initialItem: Page_User {
            id: page_ProfileView

            menuButtonVisible: true

            width: parent.width
            height: parent.height
            stackView:stackView
            navigator: navigator
            userId: wa_User.id
        }

        onVisibleChanged: {
            if(visible == true)
            {
                navigator.title = wa_User.username;
            }
        }


        onCurrentItemChanged: {
            if(currentItem == null)
                return;

            navigator.continueButtonVisible = false;
            navigator.menuButtonVisible     = true;
        }
    }
}
