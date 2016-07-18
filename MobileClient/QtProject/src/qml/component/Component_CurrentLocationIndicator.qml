/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      16.04.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick       2.5
import QtPositioning 5.5
import QtLocation    5.6

MapQuickItem {
    id: component_CurrentLocationIndicator

    coordinate: hc_LocationManager.coordinate

    anchorPoint.x: image_CurrentLocation.width *0.5
    anchorPoint.y: image_CurrentLocation.height*0.5

    sourceItem:Image {

        id: image_CurrentLocation;
        width:  18
        height: 18

        SequentialAnimation {
            loops: Animation.Infinite
            running: true
            OpacityAnimator {
                target: image_CurrentLocation;
                from: 1;
                to:   0.6;
                duration: 900
            }
            OpacityAnimator {
                target: image_CurrentLocation;
                from: 0.6;
                to:   1;
                duration: 900
            }
        }

        source: "qrc:/icon/icon/Wheel-29x29.png"
    }
}
