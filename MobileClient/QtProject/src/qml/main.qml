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

/*    // Positioning
    PositionSource {
        id: positionSource
        onPositionChanged:
        {
            if(positionSource.valid === false)
            {
                hc_Logger.slot_error("PositionSource invalid source");
                return;
            }

            if(positionSource.sourceError !== PositionSource.NoError)
            {
                hc_Logger.slot_error("PositionSource source error: " + positionSource.sourceError);
                return;
            }

            if(   position.latitudeValid  === false
               || position.longitudeValid === false)
            {
                hc_Logger.slot_error("PositionSource invalid position");
                return;
            }

            hc_LocationManager.setFakePosition(position.coordinate.latitude,
                                               position.coordinate.longitude)
        }
    }*/

    Connections{
        target: hc_LocationManager
        onSignal_RequestLocation: positionSource.update()
    }
}
