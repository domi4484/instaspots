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
#include <QScriptValue>

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

  class CONST
  {
  public:
    static const QString WEB_API_HOST;
    static const QString WEB_API_PATH;

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

  static WebApi *instance();
  static void destroy();

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

  QNetworkAccessManager m_QNetworkAccessManager;

  unsigned int m_CommandsIdCounter;
  QMap<unsigned int, WebApiCommand *> m_RunningCommands;

};

#endif // WEBAPI_H
