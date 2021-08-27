/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

// Qt includes -----------------------------
#include <QApplication>
#include <QUrl>

// Forward declarations --------------------
class Settings;
class PlateformDetail;
class ApplicationHelper;
class LocationManager;
class PictureCacher;
class CurrentUser;
class PictureUploader;
class UltraQmlAccessManagerFactory;
class QQmlApplicationEngine;

class Application : public QApplication
{
    Q_OBJECT
public:

  explicit Application(int argc,
                       char *argv[]);
  ~Application();

private slots:

  void slot_QApplication_applicationStateChanged(Qt::ApplicationState applicationState);
  void slot_QApplication_aboutToQuit();

private:

  class CONST
  {
  public:
    class SERVER
    {
    public:
      static const QString DEFAULT_ADDRESS;
    };
  };

  // Settings
  Settings                     *mSettings;

  // Plateform detail
  PlateformDetail              *mPlateformDetail;

  // Application helper
  ApplicationHelper            *mApplicationHelper;
  LocationManager              *mLocationManager;
  PictureCacher                *mPictureCacher;

  CurrentUser                  *mCurrentUser;
  PictureUploader              *mPictureUploader;

  UltraQmlAccessManagerFactory *mUltraQmlAccessManagerFactory;

  QQmlApplicationEngine        *mQQmlApplicationEngine;

  void parseCommandLineArguments();

  void saveSettings();
};

#endif // APPLICATION_H
