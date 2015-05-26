
// Project includes ------------------------
#include "Settings.h"
#include "HelperClasses/ApplicationHelper.h"
#include "HelperClasses/Logger.h"
#include "HelperClasses/PlateformDetail.h"
#include "HelperClasses/LocationManager.h"
#include "HelperClasses/PictureCacher.h"
#include "WebApi/User.h"
#include "WebApi/PictureBase.h"
#include "WebApi/NewsModel.h"
#include "WebApi/NearbySpotsModel.h"
#include "WebApi/PictureUploader.h"

// Qt includes -----------------------------
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Red Coping Company");
    app.setOrganizationDomain("spots.lowerclassclothing.com");
    app.setApplicationName("Instaspots");
    app.setApplicationVersion("0.0.0");

    Logger::instanziate(Logger::LOG_VERBOSE);

    Settings settings;

    ApplicationHelper applicationHelper;
    PlateformDetail   plateformDetail;
    LocationManager   locationManager(&settings);
    PictureCacher     pictureCacher;


    User user(&settings);
    PictureBase pictureBase;
    PictureUploader pictureUploader;
    NewsModel newsModel(&pictureBase);
    NearbySpotsModel nearbySpotModel;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("hc_Application",     &applicationHelper);
    engine.rootContext()->setContextProperty("hc_PlateformDetail", &plateformDetail);
    engine.rootContext()->setContextProperty("hc_LocationManager", &locationManager);
    engine.rootContext()->setContextProperty("hc_PictureCacher",   &pictureCacher);

    // TODO cercare setContextSingleton o qualcosa di simile
    engine.rootContext()->setContextProperty("hc_Logger",          Logger::instance());

    engine.rootContext()->setContextProperty("wa_User",            &user           );
    engine.rootContext()->setContextProperty("wa_PictureBase",     &pictureBase    );
    engine.rootContext()->setContextProperty("wa_PictureUploader", &pictureUploader);
    engine.rootContext()->setContextProperty("wa_NewsModel",       &newsModel      );
    engine.rootContext()->setContextProperty("wa_NearbySpotModel", &nearbySpotModel);

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();

    settings.sync();
}
