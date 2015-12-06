/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      05.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.2

// Project imports -------------------------
import "qrc:/"

Item{
    id: cropPicture
    visible: false

    // Navigation properties ---------------

    property string navigation_Title:                 qsTr("Crop")
    property bool   navigation_BackButtonVisible:     true
    property bool   navigation_ContinueButtonVisible: false
    property bool   navigation_MenuButtonVisible:     false


    // Properties --------------------------

    property alias source: image.source

    property alias cropX:    flick.cropX
    property alias cropY:    flick.cropY
    property alias cropSide: flick.cropSide

    property alias sourceSize: image.sourceSize
    property alias imageElement: originalImage

    property real  displayAspectRatio: height / width
    property bool  displayPortrait:    displayAspectRatio > 1.0
    property bool  displayLandscape:   !displayPortrait

    property int   transparentMargin:  10

    Image {
        id:originalImage
        visible: false
        source: cropPicture.source
    }


    // Signals -----------------------------

    signal cropFinished()


    // Gui ---------------------------------

    Flickable {
        id: flick
        width: parent.width
        height: parent.height
        contentWidth: minimumWidth
        contentHeight: minimumHeight

        property real aspectRatio: (image.sourceSize.width / image.sourceSize.height)
        property bool imagePortrait: aspectRatio > 1.0

        property real minimumWidth:  imagePortrait ? height * aspectRatio : height
        property real minimumHeight: imagePortrait ? height               : height * aspectRatio

        property real scaleFactor: image.sourceSize.width / minimumWidth;

        property real cropX: contentX * scaleFactor
        property real cropY: contentY * scaleFactor
        property real cropSide: width * scaleFactor

        Image {
            id:image
            width: flick.contentWidth
            height: flick.contentHeight

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    flick.contentWidth = flick.minimumWidth
                    flick.contentHeight = flick.minimumHeight
                }
            }
        }

//        PinchArea {
//            id: pinchArea
//            width: Math.max(flick.contentWidth, flick.width)
//            height: Math.max(flick.contentHeight, flick.height)

//            function distance(p1, p2) {
//                var dx = p2.x-p1.x;
//                var dy = p2.y-p1.y;
//                return Math.sqrt(dx*dx + dy*dy);
//            }

//            property real initialDistance
//            property real initialContentWidth
//            property real initialContentHeight

//            onPinchStarted: {
//                initialDistance = distance(pinch.point1, pinch.point2);
//                initialContentWidth = flick.contentWidth;
//                initialContentHeight = flick.contentHeight;
//            }

//            onPinchUpdated: {
//                flick.contentX += pinch.previousCenter.x - pinch.center.x
//                flick.contentY += pinch.previousCenter.y - pinch.center.y

//                var currentDistance = distance(pinch.point1, pinch.point2);
//                if(currentDistance < 5)
//                    return;
//                var scale = currentDistance/initialDistance;

//                var newHeight = initialContentHeight*scale
//                var newWidth = initialContentWidth*scale

//                flick.resizeContent(newWidth, newHeight, pinch.center)
//            }

//            onPinchFinished: {
//                var finalWidth = Math.max(flick.contentWidth, flick.minimumWidth)
//                var finalHeight = Math.max(flick.contentHeight, flick.minimumHeight)

//                //Reasure the maximum Scale
//                finalWidth = Math.min(finalWidth, image.sourceSize.width)
//                finalHeight = Math.min(finalHeight, image.sourceSize.height)

//                flick.resizeContent(finalWidth, finalHeight, pinch.center)

//                flick.returnToBounds()
//            }
//        }
    }

    // Semitransparent rectangles
    Rectangle{
        id: rectangle_Semitransparent_Top

        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.top:    parent.top

        height: displayPortrait ? (parent.height - parent.width)/2 : transparentMargin

        color:   "gray"
        opacity: 0.5
    }
    Rectangle{
        id: rectangle_Semitransparent_Left

        anchors.left:   parent.left
        anchors.top:    rectangle_Semitransparent_Top.bottom
        anchors.bottom: parent.bottom

        width: displayLandscape ? (parent.width - parent.height)/2 : transparentMargin

        color:   "gray"
        opacity: 0.5
    }
    Rectangle{
        id: rectangle_Semitransparent_Right

        anchors.right:  parent.right
        anchors.top:    rectangle_Semitransparent_Top.bottom
        anchors.bottom: parent.bottom

        width: displayLandscape ? (parent.width - parent.height)/2 : transparentMargin

        color:   "gray"
        opacity: 0.5
    }
    Rectangle{
        id: rectangle_Semitransparent_Bottom

        anchors.left:   rectangle_Semitransparent_Left.right
        anchors.right:  rectangle_Semitransparent_Right.left
        anchors.bottom: parent.bottom

        height: displayPortrait ? (parent.height - parent.width)/2 : transparentMargin

        color:   "gray"
        opacity: 0.5
    }

    Button {
        id: button_Crop
        width: parent.width / 1.1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("Crop")

        onClicked: {
            cropFinished();
        }
    }
}

