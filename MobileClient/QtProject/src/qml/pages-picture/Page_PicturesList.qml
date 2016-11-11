/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      31.03.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.3
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2

// Project c++ imports ---------------------
import PicturesModel 1.0
import Picture       1.0
import Spot          1.0

// Project qml imports ---------------------
import "qrc:/qml/"
import "qrc:/qml/views"
import "qrc:/qml/widgets/"
import "qrc:/qml/component"
import "qrc:/qml/pages-picture"

Item
{
  id: page_PicturesList


  // Bind properties ---------------------

  property alias model: picturesModel


  // Navigation properties ---------------

  property string navigation_Title:                 ""
  property bool   navigation_BackButtonVisible:     false
  property bool   navigation_ContinueButtonVisible: false
  property bool   navigation_MenuButtonVisible:     false


  // Model -------------------------------

  PicturesModel
  {
    id: picturesModel
  }


  // Signals -----------------------------

  signal userClicked(int userId, string username)
  signal spotClicked(int spotId, string spotName)
  signal likesClicked(int pictureId)


  // Methods -----------------------------

  function listView_positionViewAtBeginning()
  {
    listView.positionViewAtBeginning();
  }

  // Gui ---------------------------------

  ListView
  {
    id: listView
    anchors.fill: parent
    model: picturesModel

    delegate: PictureViewDelegate
    {
      onUserClicked:
      {
          page_PicturesList.userClicked(role_UserId, role_UserUsername);
      }
      onSpotClicked:
      {
          page_PicturesList.spotClicked(role_SpotId, role_SpotName);
      }
      onOptionsClicked:
      {
          component_ListSelectionDialog_Options.showDialog = true;
      }
      onLikesClicked:
      {
          page_PicturesList.likesClicked(role_PictureId);
      }


      // List selection dialog
      Component_ListSelectionDialog{
        id: component_ListSelectionDialog_Options

        Link
        {
          id: link_Like

          text: "Like"

          width: parent.width
          height: 30 * hc_Application.dip

          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter

          onClicked:
          {
            re_PictureRepository.likePicture(role_PictureId);
            component_ListSelectionDialog_Options.hideDialog = true;
          }
        }
        Link
        {
          id: link_Remove

          text: "Remove"

          visible: wa_CurrentUser.id == role_UserId

          width: parent.width
          height: 30 * hc_Application.dip

          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter

          onClicked:
          {
            var picture = picturesModel.getPicture(index);
            var spot = picture.spot;
            if(picture === null)
            {
              console.log("Null picture");
              component_ListSelectionDialog_Options.hideDialog = true;
              return;
            }

            // Confirmation dialog
            var component = Qt.createComponent("qrc:/qml/component/Component_Dialog.qml",
                                               Component.PreferSynchronous,
                                               link_Remove);
            var dialog = component.createObject(parent,
                                                {"title": qsTr("Warning"),
                                                 "standardButtons": MessageDialog.Ok | MessageDialog.Cancel
                                                });
            dialog.accepted.connect(removePictureCofirmed);
            dialog.rejected.connect(removePictureRejected);

            // Check last picture
            if(spot.picturesCount === 1)
            {
              dialog.text = qsTr("The spot \"%1\" has only one picture, if you remove it the spot will be removed to.").arg(spot.name);
            }
            else
            {
              dialog.text = qsTr("This picture will be removed from spot \"%1\".").arg(spot.name);
            }


            dialog.open();
          }

          function removePictureCofirmed(picture)
          {
              console.log(picturesModel.getPicture(index).id)
//              re_PictureRepository.removePicture(picturesModel.getPicture(index).id);
              component_ListSelectionDialog_Options.hideDialog = true;
          }

          function removePictureRejected()
          {
              component_ListSelectionDialog_Options.hideDialog = true;
          }
        }
      } // Component_ListSelectionDialog
    }
  }
}

