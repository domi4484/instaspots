
// Qt imports ------------------------------
import QtQuick 2.2
import QtQuick.Controls 1.1

TabWidget {
    id: tabs
    width: 640; height: 480

    Panel_Home {
        anchors.fill: parent
        property string image_IconSource: "icon/icon/folder-home.png"
    }

    Panel_NearbySpots {
        anchors.fill: parent
        property string image_IconSource: "icon/icon/find-location.png"
    }

    Panel_Upload {
        anchors.fill: parent
        property string image_IconSource: "icon/icon/camera-photo.png"
    }

    Panel_User {
        anchors.fill: parent
        property string image_IconSource: "icon/icon/im-user-offline.png"
    }
}
