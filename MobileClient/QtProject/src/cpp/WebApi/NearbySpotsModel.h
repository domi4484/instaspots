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

// Qt includes -----------------------------
#include <QAbstractListModel>

// Forward declarations --------------------
class Spot;

class NearbySpotsModel : public QAbstractListModel
{
   Q_OBJECT

public:

   explicit NearbySpotsModel(QObject *parent = 0);
    ~NearbySpotsModel();

   virtual int rowCount(const QModelIndex &parent) const;

  virtual QVariant data(const QModelIndex &index, int role) const;

  virtual QHash<int, QByteArray> roleNames() const;

  public slots:

    void setLocation(double latitude,
                     double longitude,
                     double maxDistance_km);

  private slots:
    void slot_SpotRepository_DataReady(int requestId,
                                       bool success);

  private:

    // News data
    QList<Spot * > m_QList_Spots;
    int m_RequestId;
};

#endif // NEARBYSPOTSMODEL_H

