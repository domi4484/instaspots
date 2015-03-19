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

    property string _state: ""

    onSourceUrlChanged:
    {
        var imageCached = hc_PictureCacher.isPictureCached(sourceUrl);
        if(imageCached)
        {
            _state = "cached";
        }
        else
        {
            _state = "thumb";
        }
    }

    on_StateChanged:
    {
        if(_state == "thumb")
        {
            image_Picture.sourceSize.width  = 180;
            image_Picture.sourceSize.height = 180;
            image_Picture.source = hc_PictureCacher.getThumb(sourceUrl);
        }
        else if(_state == "normal")
        {
            image_Picture.sourceSize.width  = 640;
            image_Picture.sourceSize.height = 640;
            image_Picture.source = sourceUrl;
        }
        else if(_state == "cached")
        {
            image_Picture.sourceSize.width  = 640;
            image_Picture.sourceSize.height = 640;
            // TODO  image_Picture.source = hc_PictureCacher.getCached(sourceUrl);;
            image_Picture.source = sourceUrl;
        }

    }

    Image {
        id: image_Picture
        anchors.fill: parent

        onStatusChanged:
        {
            if(status == Image.Ready)
            {
                if(cachedPicture._state == "thumb")
                {
                    cachedPicture._state = "normal";
                }
                else if(cachedPicture._state == "normal")
                {
                    // TODO Cachare l'immagine
                    console.log("Download finished");
                }
            }
        }
    }
}
