/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      31.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef NEARBYSPOTSMODEL_H
#define NEARBYSPOTSMODEL_H

// Projects includes -----------------------
#include "Spot.h"
#include "WebApiCommand.h"
#include "WebApiError.h"

// Qt includes -----------------------------
#include <QAbstractListModel>

// Forward declarations --------------------
class PictureBase;

class NearbySpotsModel : public QAbstractListModel
{
  Q_OBJECT

  public:

    explicit NearbySpotsModel(QObject *parent = 0);
    ~NearbySpotsModel();

    virtual int rowCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QHash<int, QByteArray> roleNames() const;

  signals:

  public slots:

    void setLocation(float latitude,
                     float longitude);

  private slots:
    void slot_CommandGetNearbySpots_Finished (const WebApiError &error);

  private:

    static const QString C_GET_NEARBY_SPOTS;

    static const QString R_PARAM_LATITUDE;
    static const QString R_PARAM_LONGITUDE;

    static const QString A_ARRAY_SPOTS;

    static const QString A_ARRAY_SPOTS_ELEMENT_ID;
    static const QString A_ARRAY_SPOTS_ELEMENT_NAME;
    static const QString A_ARRAY_SPOTS_ELEMENT_DESCRIPTION;
    static const QString A_ARRAY_SPOTS_ELEMENT_LATITUDE;
    static const QString A_ARRAY_SPOTS_ELEMENT_LONGITUDE;
    static const QString A_ARRAY_SPOTS_ELEMENT_DISTANCE;
    static const QString A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_1;
    static const QString A_ARRAY_SPOTS_ELEMENT_PICTURE_URL_2;

    // News data
    QList<Spot * > m_QList_Spots;

    WebApiCommand m_Command_GetNearbySpots;
};

#endif // NEARBYSPOTSMODEL_H

