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

// Forward declarations --------------------
class Settings;
class PlateformDetail;
class ApplicationHelper;
class LocationManager;
class PictureCacher;
class User;
class PictureUploader;
class QQmlApplicationEngine;

class Application : public QApplication
{
    Q_OBJECT
public:

  explicit Application(int argc,
                       char *argv[]);
  ~Application();

private:

  static const QString CONST_COMMANDLINEARGUMENT_DEVELOPMENTMODE;

  // Settings
  Settings              *m_Settings;

  // Plateform detail
  PlateformDetail       *m_PlateformDetail;

  // Application helper
  ApplicationHelper     *m_ApplicationHelper;
  LocationManager       *m_LocationManager;
  PictureCacher         *m_PictureCacher;

  User                  *m_User;
  PictureUploader       *m_PictureUploader;

  QQmlApplicationEngine *m_QQmlApplicationEngine;

  QMap<QString, QVariant> parseCommandLineArguments();
};

#endif // APPLICATION_H
