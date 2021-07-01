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
  Settings                     *m_Settings;

  // Plateform detail
  PlateformDetail              *m_PlateformDetail;

  // Application helper
  ApplicationHelper            *m_ApplicationHelper;
  LocationManager              *m_LocationManager;
  PictureCacher                *m_PictureCacher;

  CurrentUser                  *m_CurrentUser;
  PictureUploader              *m_PictureUploader;

  UltraQmlAccessManagerFactory *m_UltraQmlAccessManagerFactory;

  QQmlApplicationEngine        *m_QQmlApplicationEngine;

  QMap<QString, QVariant> parseCommandLineArguments();

  void saveSettings();
};

#endif // APPLICATION_H
