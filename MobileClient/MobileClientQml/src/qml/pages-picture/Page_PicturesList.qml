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
import QtQuick 2.5
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
      id: pictureViewDelegate

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
        var picture = picturesModel.getPicture(index);
        var spot = picture.spot;
        if(picture === null)
        {
          console.log("Null picture");
          return;
        }

        var component_ListSelectionDialog = Qt.createComponent("qrc:/qml/component/Component_ListSelectionDialog.qml",
                                           Component.PreferSynchronous,
                                           pictureViewDelegate);
        var object_ListSelectionDialog = component_ListSelectionDialog.createObject(pictureViewDelegate);

        var array_Links = new Array();

        // Like / Unlike
        if(picture.likedByUserId(wa_CurrentUser.id) === false)
        {
          var component_Link_Like = Qt.createComponent("qrc:/qml/widgets/Link.qml",
                                                       Component.PreferSynchronous,
                                                       object_ListSelectionDialog);
          var object_Link_Like = component_Link_Like.createObject(object_ListSelectionDialog,
                                                                  {
                                                                    "text"                : qsTr("Like"),
                                                                    "width"               : parent.width,
                                                                    "height"              : 30 * hc_Application.dip,
                                                                    "horizontalAlignment" : Text.AlignHCenter,
                                                                    "verticalAlignment"   : Text.AlignVCenter
                                                                  });
          object_Link_Like.clicked.connect(object_ListSelectionDialog.close)
          object_Link_Like.clicked.connect(link_Like_Clicked)

          array_Links.push(object_Link_Like);
        }
        else
        {
          // UnLike
          var component_Link_UnLike = Qt.createComponent("qrc:/qml/widgets/Link.qml",
                                                         Component.PreferSynchronous,
                                                         object_ListSelectionDialog);
          var object_Link_UnLike = component_Link_UnLike.createObject(object_ListSelectionDialog,
                                                                      {
                                                                        "text"                : qsTr("Unlike"),
                                                                        "width"               : parent.width,
                                                                        "height"              : 30 * hc_Application.dip,
                                                                        "horizontalAlignment" : Text.AlignHCenter,
                                                                        "verticalAlignment"   : Text.AlignVCenter
                                                                      });
          object_Link_UnLike.clicked.connect(object_ListSelectionDialog.close)
          object_Link_UnLike.clicked.connect(link_UnLike_Clicked)

          array_Links.push(object_Link_UnLike);
        }

        // Remove
        if(picture.username === wa_CurrentUser.username)
        {
          var component_Link_Remove = Qt.createComponent("qrc:/qml/widgets/Link.qml",
                                                         Component.PreferSynchronous,
                                                         object_ListSelectionDialog);
          var object_Link_Remove = component_Link_Remove.createObject(object_ListSelectionDialog,
                                                                      {
                                                                        "text"                : qsTr("Remove"),
                                                                        "width"               : parent.width,
                                                                        "height"              : 30 * hc_Application.dip,
                                                                        "horizontalAlignment" : Text.AlignHCenter,
                                                                        "verticalAlignment"   : Text.AlignVCenter
                                                                      });
          object_Link_Remove.clicked.connect(object_ListSelectionDialog.close)
          object_Link_Remove.clicked.connect(link_Remove_Clicked)

          array_Links.push(object_Link_Remove);
      }

        object_ListSelectionDialog.content = array_Links;
        object_ListSelectionDialog.showDialog = true;
      }
      onLikesClicked:
      {
        page_PicturesList.likesClicked(role_PictureId);
      }

      function link_Like_Clicked()
      {
        re_PictureRepository.likePicture(picturesModel.getPicture(index).id);
      }

      function link_UnLike_Clicked()
      {
        re_PictureRepository.unlikePicture(picturesModel.getPicture(index).id);
      }

      function link_Remove_Clicked()
      {
        var picture = picturesModel.getPicture(index);
        var spot = picture.spot;

        // Confirmation dialog
        var component = Qt.createComponent("qrc:/qml/component/Component_Dialog.qml",
                                           Component.PreferSynchronous,
                                           parent);
        var dialog = component.createObject(parent,
                                            {"title": qsTr("Warning"),
                                             "standardButtons": MessageDialog.Ok | MessageDialog.Cancel
                                            });
        dialog.accepted.connect(link_Remove_Clicked_RemoveConfirmed);

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

      function link_Remove_Clicked_RemoveConfirmed(picture)
      {
        re_PictureRepository.removePicture(picturesModel.getPicture(index).id);
      }
    }
  }
}

