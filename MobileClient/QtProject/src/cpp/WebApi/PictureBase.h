/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      03.11.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef COMMAND_GETSPOTS_H
#define COMMAND_GETSPOTS_H

// Project includes ------------------------
#include "WebApiCommand.h"
#include "Picture.h"

// Qt includes -----------------------------
#include <QMap>

class PictureBase : public QObject
{

  Q_OBJECT

public:

  explicit PictureBase(QObject *parent = 0);

  virtual ~PictureBase();

  void clear();

  void execute();

  QList<int> getPicturesId();
  QList<Picture *> getPictures();

  void getNearbySpots();

signals:

  void signal_News(const WebApiError &,
                   const QList<Picture *> &);

  public slots:

  private slots:

    void slot_CommandGetNews_Finished        (const WebApiError &error);

  private:

    static const QString COMMAND;
    static const QString C_GET_NEWS;

    static const QString A_ARRAY_PICTURES;
    static const QString A_ARRAY_PICTURES_ELEMENT_ID;
    static const QString A_ARRAY_PICTURES_ELEMENT_ID_USER;
    static const QString A_ARRAY_PICTURES_ELEMENT_ID_SPOT;
    static const QString A_ARRAY_PICTURES_ELEMENT_URL;
    static const QString A_ARRAY_PICTURES_ELEMENT_USERNAME;
    static const QString A_ARRAY_PICTURES_ELEMENT_NAME;
    static const QString A_ARRAY_PICTURES_ELEMENT_DESCRIPTION;
    static const QString A_ARRAY_PICTURES_ELEMENT_CREATED;

    QMap<int, Picture * > m_QMap_Pictures;

    WebApiCommand m_Command_GetNews;
};

#endif // COMMAND_GETSPOTS_H
