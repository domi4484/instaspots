/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      13.12.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtPositioning 5.2
import QtLocation    5.6

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/widgets"


ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 340
    height: 500

    // Signals -----------------------------

    signal pushMainGui


    // Slots -------------------------------

    Component.onCompleted:{

        // Push the application main gui
        pushMainGui();
    }

    Connections{
        target: hc_Application
        onSignal_NewClientVersionAvailable:
        {
            messageDialog_NewClientVersionAvailable.open();
        }
    }

    // Push the application main gui
    onPushMainGui: stackView.push({immediate : true,
                                   replace   : true,
                                   item      : Qt.resolvedUrl("qrc:/qml/TabWidgetBottom_Main.qml")});


    // Gui ---------------------------------
    StackView {
        id: stackView

        anchors.fill: parent

        initialItem: Image {
            id: image_Splash
            width : parent.width
            height: parent.height

            source: "qrc:/icon/icon/splash.png"
        }
    }


    // Message dialogs ---------------------
    MessageDialog{
        id: messageDialog_NewClientVersionAvailable
        title: qsTr('New version out!')
        text: qsTr('There is a new Lowerspot app version available for download! Get it on lowerspot.com!')

        onAccepted:
        {
            hc_Application.newerClientVersionAvailableGotIt();
        }
    }



    Plugin {
        id: plugin_MapBox
        name: "mapbox"
        PluginParameter { name: "mapbox.access_token"; value: hc_LocationManager.mapboxAccessToken() }
        PluginParameter { name: "mapbox.map_id";       value: "mapbox.streets" }
    }
}
