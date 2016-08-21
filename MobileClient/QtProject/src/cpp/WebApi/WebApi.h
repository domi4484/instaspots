/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      19.05.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef WEBAPI_H
#define WEBAPI_H

// Qt includes -----------------------------
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

// Project includes ------------------------
#include "QueryItem.h"
#include "../HelperClasses/UltraNetworkAccessManager.h"

// Forward declarations --------------------
class WebApiCommand;
class QFile;

class WebApi : public QObject
{
  Q_OBJECT
public:

  static const QString URL_DEVELOPMENT;
  static const QString URL_PRODUCTION;

  class CONST
  {
  public:

    class GENERAL_PARAMS
    {
    public:
      static const QString COMMAND;
      static const QString ERROR;
    };
  }; // CONST

  // Commands
  class COMMAND
  {
  public:
    static const QString LOGIN;
    static const QString LOGOUT;
    static const QString CANREGISTER;
    static const QString REGISTER;

    static const QString REPORT_PROBLEM;
    static const QString GET_CURRENT_CLIENT_VERSION;

    static const QString UPLOAD_PICTURE_TO_SPOT;
    static const QString UPLOAD_NEW_SPOT;

    static const QString GET_PICTURES_BY_NEWEST;
    static const QString GET_PICTURES_BY_SPOT_ID;
    static const QString GET_PICTURES_BY_USER_ID;
    static const QString GET_SPOTS_BY_DISTANCE;
    static const QString GET_SPOTS_BY_USER_ID;

    static const QString REMOVE_PICTURE;
  }; // COMMAND

  // Paramters
  class PARAMETER
  {
  public:
    static const QString USER_USER_ID;
    static const QString USER_USERNAME;
    static const QString USER_PASSWORD;
    static const QString USER_EMAIL;
    static const QString USER_AUTHENTICATED;
    static const QString USER_REGISTERED;

    static const QString REPORT_TITLE;
    static const QString REPORT_CONTENT;

    static const QString APPLICATION_VERSION;
    static const QString PARAM_SUCCESSFUL;

    static const QString SPOT_LIST;
    static const QString SPOT_SPOT_ID;
    static const QString SPOT_NAME;
    static const QString SPOT_DESCRIPTION;
    static const QString SPOT_SECRET_SPOT;
    static const QString SPOT_LATITUDE;
    static const QString SPOT_LONGITUDE;
    static const QString SPOT_DISTANCE_KM;
    static const QString SPOT_PICTURE_PICTURE_ID_1;
    static const QString SPOT_PICTURE_PICTURE_ID_2;
    static const QString SPOT_PICTURE_URL_1;
    static const QString SPOT_PICTURE_URL_2;

    static const QString PICTURE_LIST;
    static const QString PICTURE_PICTURE_ID;
    static const QString PICTURE_LATITUDE;
    static const QString PICTURE_LONGITUDE;
    static const QString PICTURE_URL;
    static const QString PICTURE_CREATED;
    static const QString PICTURE_SPOT_ID;
    static const QString PICTURE_SPOT_NAME;
    static const QString PICTURE_SPOT_DESCRIPTION;
    static const QString PICTURE_SPOT_SCORE;
    static const QString PICTURE_USER_ID;
    static const QString PICTURE_USER_USERNAME;

    static const QString A_PARAM_SUCCESSFUL;
  }; // PARAMETER

  static WebApi *instance();
  static void destroy();

  void setUrl(const QString url);

  void postRequest(WebApiCommand *abstractCommand,
                   const QList<QueryItem> &qList_QueryItems);
  void multipartRequest(WebApiCommand *abstractCommand,
                        const QList<QueryItem> &qList_QueryItems,
                        QIODevice *device);
  void downloadRequest(WebApiCommand *abstractCommand,
                       const QString &url);

private slots:

  void slot_QNetworkReply_finished();
  void slot_QNetworkReply_uploadProgress(qint64 received,
                                         qint64 total);
  void slot_QNetworkReply_downloadProgress(qint64 received,
                                           qint64 total);

private:

   // Construcors are privates (singleton)
  explicit WebApi();
  virtual ~WebApi();

  static const char *PROPERTY_COMMAND_ID;

  static WebApi *s_Instance;

  QUrl m_Url;

  UltraNetworkAccessManager m_UltraNetworkAccessManager;

  unsigned int m_CommandsIdCounter;
  QMap<unsigned int, WebApiCommand *> m_RunningCommands;

};

#endif // WEBAPI_H
