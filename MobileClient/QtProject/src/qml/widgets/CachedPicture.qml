/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      02.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.0

Item {

    id: cachedPicture

    property string sourceUrl: ""

    property bool _cached: false
    property bool _thumb: false

    onSourceUrlChanged:
    {
        _cached = hc_PictureCacher.isPictureCached(sourceUrl);
        if(_cached)
        {
            image_Picture.sourceSize.width  = 640;
            image_Picture.sourceSize.height = 640;
            // TODO  image_Picture.source = hc_PictureCacher.getCached(sourceUrl);;
            image_Picture.source = sourceUrl;
        }
        else if(width <= 180)
        {
            _thumb = true;
            image_Picture.sourceSize.width  = 180;
            image_Picture.sourceSize.height = 180;
            image_Picture.source =  hc_PictureCacher.getThumb(sourceUrl);
        }

        else
        {
            image_Picture.sourceSize.width  = 640;
            image_Picture.sourceSize.height = 640;
            image_Picture.source = sourceUrl;
        }
    }

    Image {
        id: image_Picture
        anchors.fill: parent

        onStatusChanged:
        {
            if(status != Image.Ready)
                return;

            if(cachedPicture._thumb)
            {
                return;
            }

            if(cachedPicture._cached)
                return;

            // TODO Cachare l'immagine
            // console.log("Download finished");
        }
    }

    Rectangle {
        anchors.fill: parent

        radius: 2
        color: "transparent"
        border.width: 1
        border.color: "white"
    }
}
