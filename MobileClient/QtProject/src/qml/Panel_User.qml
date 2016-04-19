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
import QtQuick 2.3
import QtQuick.Controls 1.2

// Project qml imports ---------------------
import "qrc:/qml/views"
import "qrc:/qml/pages-user"

Item {

    // Components --------------------------

    Page_Settings {
        id: page_Settings
        visible: false
    }

    Page_User {
        id: page_User
        width: parent.width
        height: parent.height

        // Properties
        userId: wa_User.id

        // Navigation properties
        navigation_Title:                 wa_User.username
        navigation_BackButtonVisible:     false
        navigation_ContinueButtonVisible: false
        navigation_MenuButtonVisible:     true
    }


    // Gui ---------------------------------

    Navigator{
        id: navigator

        anchors.top: parent.top
        width:       parent.width

        title                 : (stackView.currentItem != null) ? stackView.currentItem.navigation_Title                 : "";
        backButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_BackButtonVisible     : false;
        continueButtonVisible : (stackView.currentItem != null) ? stackView.currentItem.navigation_ContinueButtonVisible : false;
        menuButtonVisible     : (stackView.currentItem != null) ? stackView.currentItem.navigation_MenuButtonVisible     : false;

        onPreviousPage: {
            if(stackView.depth > 1)
                stackView.pop();
        }

        onMenuClicked: {
            stackView.push(Qt.resolvedUrl("qrc:/qml/pages-user/Page_Settings.qml"));
        }
    }

    StackView {
        id: stackView

        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.top:    navigator.bottom
        anchors.bottom: parent.bottom

        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true;
                         }
        initialItem: page_User
    }


    // Slots -------------------------------

    onVisibleChanged: {
        if(visible == false)
            return;

        if(wa_User.isConnected() === false)
        {
            var page_SignIn = Qt.resolvedUrl("qrc:/qml/pages-user/Page_SignIn.qml");
            stackView.push( {
                               item       : page_SignIn,
                               immediate  : true,
                               replace    : false,
                               properties : { stackView : stackView }
                             } );
            return;
        }
    }
}
