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

Rectangle {

    onVisibleChanged: {
        if(visible == false)
        {
            return;
        }

        hc_LocationManager.requestLocation();
        text_Latitude.text = hc_LocationManager.latitude();
        text_Longitude.text = hc_LocationManager.longitude();
    }

    Connections {
        target: hc_LocationManager
        onUpdate:
        {
            if(timeout)
            {
                text_Error.text = "Timeout"
            }
            else
            {
                text_Error.text = "NoTimeout"
                text_Latitude.text = hc_LocationManager.latitude();
                text_Longitude.text = hc_LocationManager.longitude();
            }
        }
    }

    Column{
    Text {
        id: text_Latitude
        text: "Latitude"
    }
    Text {
        id: text_Longitude
        text: "Longitude"
    }
    Text {
        id: text_Error
        text: "noerror"
    }
    }


//    // Gui ---------------------------------
//    Navigator{
//        id: navigator
//        anchors.top: parent.top

//        backButtonVisible: stackView.depth > 1
//        continueButtonVisible: stackView.currentItem.continueButtonVisible

//        onPreviousPage: {
//            if(stackView.depth > 1)
//                stackView.pop();
//        }

//        onContinueClicked: {
//            stackView.currentItem.continueClicked();
//        }
//    }

//    StackView {
//        id: stackView
//        anchors.top: navigator.bottom
//        anchors.bottom: parent.bottom
//        // Implements back key navigation
//        focus: true
//        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
//                             stackView.pop();
//                             event.accepted = true;
//                         }
//        initialItem: ProfileView {
//            id: page_ProfileView
//            width: parent.width
//            height: parent.height

//            title: wa_User.username
//        }

//        onCurrentItemChanged: {
//            if(currentItem == null)
//                return;

//            navigator.title = currentItem.title;
//            navigator.continueButtonVisible = currentItem.continueButtonVisible;
//        }
//    }
}
