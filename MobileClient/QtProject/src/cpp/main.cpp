
// Project includes ------------------------
#include "Settings.h"
#include "HelperClasses/ApplicationHelper.h"
#include "HelperClasses/Logger.h"
#include "HelperClasses/PlateformDetail.h"
#include "HelperClasses/LocationManager.h"
#include "HelperClasses/PictureCacher.h"
#include "WebApi/SpotRepository.h"
#include "WebApi/PictureRepository.h"
#include "WebApi/PicturesModel.h"
#include "WebApi/User.h"
#include "WebApi/NewsModel.h"
#include "WebApi/NearbySpotsModel.h"
#include "WebApi/PictureUploader.h"

// Qt includes -----------------------------
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Red Coping Company");
    app.setOrganizationDomain("spots.lowerclassclothing.com");
    app.setApplicationName("Lowerspot");
    app.setApplicationVersion("0.0.2");

    Logger::instanziate(Logger::LOG_VERBOSE);

    Settings settings;

    ApplicationHelper applicationHelper;
    PlateformDetail   plateformDetail;
    LocationManager   locationManager(&settings);
    PictureCacher     pictureCacher;

    PictureRepository::instanziate();
    SpotRepository::instanziate();

    User user(&settings);
    PictureUploader pictureUploader;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("hc_Application",     &applicationHelper);
    engine.rootContext()->setContextProperty("hc_PlateformDetail", &plateformDetail);
    engine.rootContext()->setContextProperty("hc_LocationManager", &locationManager);
    engine.rootContext()->setContextProperty("hc_PictureCacher",   &pictureCacher);

    // TODO cercare setContextSingleton o qualcosa di simile
    engine.rootContext()->setContextProperty("hc_Logger",          Logger::instance());

    engine.rootContext()->setContextProperty("wa_User",              &user           );
    engine.rootContext()->setContextProperty("wa_PictureUploader",   &pictureUploader);

    qmlRegisterType<PicturesModel>   ("PicturesModel",    1, 0, "PicturesModel");
    qmlRegisterType<NewsModel>       ("NewsModel",        1, 0, "NewsModel");
    qmlRegisterType<NearbySpotsModel>("NearbySpotsModel", 1, 0, "NearbySpotsModel");


    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();

    settings.sync();

    Logger::destroy();
    SpotRepository::destroy();
    PictureRepository::destroy();
}
