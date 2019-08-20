/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.05.2016                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Qt imports ------------------------------
import QtQuick 2.5

Item {

    property string tabWidgetDynamicTab_ContentSourceUrl : ""
    property bool   tabWidgetDynamicTab_IsLoaded         : false

    property bool   tabWidget_IsDynamicTab : true

    Loader {
      id: loader

      anchors.fill: parent
    }

    function tabWidgetDynamicTab_Load()
    {
        loader.source = tabWidgetDynamicTab_ContentSourceUrl;
        tabWidgetDynamicTab_IsLoaded = true;
    }

    function tabWidget_CurrentTabClicked()
    {
        if(loader.item.tabWidget_CurrentTabClicked != null)
        {
            loader.item.tabWidget_CurrentTabClicked();
        }
    }

    function tabWidget_CurrentTabReclicked()
    {
        if(loader.item.tabWidget_CurrentTabReclicked != null)
        {
            loader.item.tabWidget_CurrentTabReclicked();
        }
    }
}
