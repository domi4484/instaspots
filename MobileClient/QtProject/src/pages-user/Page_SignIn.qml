/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
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
import "qrc:/"
import "qrc:/widgets"

BasicPage{
    id: page_SignIn

    // BasicPage properties ----------------
    continueButtonVisible: false
    menuButtonVisible: false
    title: qsTr("Sign in")

    // Properties --------------------------
    property var navigator
    property StackView stackView

    // Signals -----------------------------

    // Slots -------------------------------
    Component.onCompleted: {
        navigator.title = title;
    }

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

    // Components --------------------------
    Component {
        id: component_SignIn
        Item {
        }
    }

    Component {
        id: component_Register
        Item {
        }
    }

    // Gui ---------------------------------
    TabWidgetTop {
        id: tabWidget
        width: parent.width
        height: parent.height

        Page_Login {
            anchors.fill: parent
            property string tabWidget_ButtonText: "Login"
            property string tabWidget_ButtonIconSource: ""
        }

        Page_Register {
            anchors.fill: parent
            property string tabWidget_ButtonText: "Register"
            property string tabWidget_ButtonIconSource: ""
        }
    }
}
