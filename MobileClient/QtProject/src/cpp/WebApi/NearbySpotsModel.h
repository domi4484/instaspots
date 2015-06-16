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

    // News data
    QList<Spot * > m_QList_Spots;

    WebApiCommand m_Command_GetNearbySpots;
};

#endif // NEARBYSPOTSMODEL_H

