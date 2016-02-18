
// Project includes ------------------------
#include "Settings/Settings.h"
#include "HelperClasses/ApplicationHelper.h"
#include "HelperClasses/Logger.h"
#include "HelperClasses/PlateformDetail.h"
#include "HelperClasses/LocationManager.h"
#include "HelperClasses/PictureCacher.h"
#include "WebApi/SpotRepository.h"
#include "WebApi/SpotsModel.h"
#include "WebApi/Spot.h"
#include "WebApi/PictureRepository.h"
#include "WebApi/PicturesModel.h"
#include "WebApi/Picture.h"
#include "WebApi/User.h"
#include "WebApi/PictureUploader.h"

// Qt includes -----------------------------
#include <QApplication>
#include <QMap>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

//-----------------------------------------------------------------------------------------------------------------------------

const QString CONST_COMMANDLINEARGUMENT_DEVELOPMENTMODE("developmentMode");

//-----------------------------------------------------------------------------------------------------------------------------

QMap<QString, QVariant> parseCommandLineArguments();

//-----------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName   ("Lowerspot");
    app.setOrganizationDomain ("lowerspot.com");
    app.setApplicationName    ("Lowerspot");
    app.setApplicationVersion ("V0.0.4");

    // Command line arguments
    QMap<QString, QVariant> qMap_Arguments = parseCommandLineArguments();

    // Settings
    Settings settings;

    // Logger
    Logger::instanziate(Logger::LOG_VERBOSE);
    Logger::instance()->setLogLevel(settings.get_Logger_LogLevel());

    // Plateform detail
    PlateformDetail   plateformDetail;

    // Application helper
    ApplicationHelper applicationHelper(&settings,
                                        &plateformDetail);
    applicationHelper.setDevelopmentMode(qMap_Arguments.value(CONST_COMMANDLINEARGUMENT_DEVELOPMENTMODE).toBool());
    applicationHelper.startupTimerStart();

    LocationManager   locationManager(&settings,
                                      &plateformDetail);
    PictureCacher     pictureCacher;

    PictureRepository::instanziate();
    SpotRepository::instanziate(&locationManager);

    User user(&settings);
    PictureUploader pictureUploader;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("hc_Application",     &applicationHelper);
    engine.rootContext()->setContextProperty("hc_PlateformDetail", &plateformDetail);
    engine.rootContext()->setContextProperty("hc_LocationManager", &locationManager);
    engine.rootContext()->setContextProperty("hc_PictureCacher",   &pictureCacher);
    engine.rootContext()->setContextProperty("hc_Logger",          Logger::instance());
    engine.rootContext()->setContextProperty("hc_Settings",        &settings);

    engine.rootContext()->setContextProperty("wa_User",              &user           );
    engine.rootContext()->setContextProperty("wa_PictureUploader",   &pictureUploader);

    engine.rootContext()->setContextProperty("re_PictureRepository", PictureRepository::instance());
    engine.rootContext()->setContextProperty("re_SpotRepository",    SpotRepository::instance());

    qmlRegisterType<PicturesModel>   ("PicturesModel",    1, 0, "PicturesModel");
    qmlRegisterType<Picture>         ("Picture",          1, 0, "Picture");
    qmlRegisterType<SpotsModel>      ("SpotsModel",       1, 0, "SpotsModel");
    qmlRegisterType<Spot>            ("Spot",             1, 0, "Spot");


    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    int exitCode = app.exec();

    Logger::info("Application closing...");

    // Set and save settings
    settings.set_Application_LastVersion(app.applicationVersion());
    settings.set_Logger_LogLevel(Logger::instance()->getLogLevel());
    settings.sync();

    Logger::destroy();
    SpotRepository::destroy();
    PictureRepository::destroy();

    return exitCode;
}

//-----------------------------------------------------------------------------------------------------------------------------

QMap<QString, QVariant> parseCommandLineArguments()
{
    QCommandLineParser qCommandLineParser;
    qCommandLineParser.setApplicationDescription(QApplication::applicationName());
    qCommandLineParser.addHelpOption();
    qCommandLineParser.addVersionOption();

    // Development mode
    QCommandLineOption qCommandLineOption_development(QStringList() << "d" << "development",
                                                      "Run in development mode.");
    qCommandLineParser.addOption(qCommandLineOption_development);

    qCommandLineParser.process(QApplication::arguments());

    QMap<QString, QVariant> qMap_Arguments;

    qMap_Arguments.insert(CONST_COMMANDLINEARGUMENT_DEVELOPMENTMODE, qCommandLineParser.isSet(qCommandLineOption_development));

    return qMap_Arguments;
}
