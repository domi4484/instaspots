/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      17.09.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.4
import QtQuick.Controls 1.3

// Project c++ imports ---------------------

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"

Item{
    id: page_SignIn


    // Properties --------------------------

    property var navigator
    property StackView stackView


    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Sign in")
    property bool   navigation_BackButtonVisible:     false
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Connections -------------------------

    Connections {
        target: wa_User
        onSignal_LoginSuccessfull:
        {
            // Login successfull
            if(success)
            {
                stackView.pop({immediate: true});
                return;
            }
        }
    }


    // Gui ---------------------------------

    TabWidget {
        id: tabWidget
        width: parent.width
        height: parent.height

        Page_Login {
            anchors.fill: parent

            // TabWidget properties
            property string tabWidget_ButtonText: qsTr("Login")
            property string tabWidget_ButtonIconSource: ""
        }

        Page_Register {
            anchors.fill: parent

            // TabWidget properties
            property string tabWidget_ButtonText: qsTr("Register")
            property string tabWidget_ButtonIconSource: ""
        }
    }
}
