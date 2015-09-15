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

// Forward declarations --------------------
class WebApiCommand;
class QNetworkAccessManager;
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

    class GET_PICTURES
    {
    public:
      static const QString COMMAND;
      static const QString A_ARRAY_ID;
    }; // LATEST_PICTURES_ID
  }; // CONST

  static const QString C_LOGIN;
  static const QString C_LOGOUT;
  static const QString C_REGISTER;
  static const QString C_CANREGISTER;
  static const QString C_GET_NEARBY_SPOTS;
  static const QString C_GET_PICTURES_BY_SPOT_ID;
  static const QString C_GET_PICTURES_BY_USER_ID;
  static const QString C_GET_NEWS;

  static const QString R_PARAM_USER_ID;
  static const QString R_PARAM_USERNAME;
  static const QString R_PARAM_PASSWORD;
  static const QString R_PARAM_EMAIL;

  static const QString R_PARAM_SPOT_ID;

  static const QString R_PARAM_LATITUDE;
  static const QString R_PARAM_LONGITUDE;
  static const QString R_PARAM_MAX_DISTANCE_KM;

  static const QString A_PARAM_AUTHENTICATION;
  static const QString A_PARAM_REGISTERED;
  static const QString A_PARAM_ID_USER;

  static const QString A_ARRAY_SPOTS;
  static const QString A_ARRAY_SPOTS_ELEMENT_ID;
  static const QString A_ARRAY_SPOTS_ELEMENT_NAME;
  static const QString A_ARRAY_SPOTS_ELEMENT_DESCRIPTION;
  static const QString A_ARRAY_SPOTS_ELEMENT_LATITUDE;
  static const QString A_ARRAY_SPOTS_ELEMENT_LONGITUDE;
  static const QString A_ARRAY_SPOTS_ELEMENT_DISTANCE_KM;
  static const QString A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1;
  static const QString A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2;

  static const QString A_ARRAY_PICTURES;
  static const QString A_ARRAY_PICTURES_ELEMENT_ID;
  static const QString A_ARRAY_PICTURES_ELEMENT_ID_USER;
  static const QString A_ARRAY_PICTURES_ELEMENT_ID_SPOT;
  static const QString A_ARRAY_PICTURES_ELEMENT_URL;
  static const QString A_ARRAY_PICTURES_ELEMENT_USERNAME;
  static const QString A_ARRAY_PICTURES_ELEMENT_NAME;
  static const QString A_ARRAY_PICTURES_ELEMENT_DESCRIPTION;
  static const QString A_ARRAY_PICTURES_ELEMENT_CREATED;


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

  QNetworkAccessManager m_QNetworkAccessManager;

  unsigned int m_CommandsIdCounter;
  QMap<unsigned int, WebApiCommand *> m_RunningCommands;

};

#endif // WEBAPI_H
