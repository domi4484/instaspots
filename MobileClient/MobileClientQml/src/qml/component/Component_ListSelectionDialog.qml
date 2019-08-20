/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      21.08.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.0
import QtQuick.Controls 1.3

// Project qml imports ---------------------
import "qrc:/qml/widgets/"

Item{

     id: component_ListSelectionDialog     

     default property alias content: column_Content.children

     anchors.fill: parent

     opacity: 0
     visible: opacity != 0

     property alias showDialog: showDialog.running
     property alias hideDialog: hideDialog.running

     PropertyAnimation { id: showDialog; target: component_ListSelectionDialog; property: "opacity"; to: 1; duration: 500; easing.type: Easing.InQuad}
     PropertyAnimation { id: hideDialog; target: component_ListSelectionDialog; property: "opacity"; to: 0; duration: 500; easing.type: Easing.OutQuad}

     Rectangle{

         id: rectangle_Background

         opacity: 0.5

         visible: component_ListSelectionDialog.opacity > 0

         anchors.fill: parent

         MouseArea {
             anchors.fill: parent
             onClicked: hideDialog.running = true
         }
     }

     Rectangle{

         id: rectangle_Dialog

         anchors.centerIn: parent

         height: column_Content.children.length * 30 *hc_Application.dip
         width: parent.width - 20 * hc_Application.dip

         radius: 10 * hc_Application.dip
         color: "white"

         Column {
             id: column_Content

             anchors.fill: parent
         }
     }

     function open()
     {
       component_ListSelectionDialog.showDialog = true;
     }

     function close()
     {
        component_ListSelectionDialog.hideDialog = true;
     }
}

