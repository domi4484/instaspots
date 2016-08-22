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

Item{

     id: component_ListSelectionDialog

     anchors.fill: parent

     opacity: 0

     signal itemClicked (string itemText, int index)

     property alias showDialog: showDialog.running
     property alias hideDialog: hideDialog.running

     property alias model: listView_Dialog.model

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

         height: listView_Dialog.height + 4
         width: parent.width - 20

         radius: 10
         color: "white"

         ListView{
             id: listView_Dialog

             width: parent.width
             height:model.count*(itemHeight+spacing)

             property int itemHeight: 30

             spacing: 4
             interactive: false
             anchors.top: rectangle_Dialog.top
             anchors.topMargin: 4
             onModelChanged: {
                 listView_Dialog.height = listView_Dialog.model.count*(itemHeight+spacing)
             }

             delegate: Rectangle{
                 id: listitem
                 width: parent.width - 20
                 height: listView_Dialog.itemHeight
                 radius: 10
                 anchors.horizontalCenter: parent.horizontalCenter
                 color: mouseArea_Delegate.pressed ? Qt.lighter(hc_Application.color_BackgroundNavigator(), 1.2)
                                                   : hc_Application.color_BackgroundNavigator()

                 Text {
                     id: listitemText
                     text: itemText
                     anchors.centerIn: parent
                     font.pixelSize: 14
                     color: "white"
                 }

                 MouseArea{
                    id: mouseArea_Delegate
                    anchors.fill: parent
                    onClicked: {
                        component_ListSelectionDialog.itemClicked(itemText, index)
                        component_ListSelectionDialog.hideDialog = true
                    }
                 }
             }
         }
     }
}

