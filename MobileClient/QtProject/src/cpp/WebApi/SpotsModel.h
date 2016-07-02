/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      19.10.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef SPOTSMODEL_H
#define SPOTSMODEL_H


// Qt includes -----------------------------
#include <QAbstractListModel>
#include <QGeoCoordinate>

// Forward declarations --------------------
class Spot;

class SpotsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( int            count       READ rowCount                       NOTIFY countChanged              )

public:

  class _CONST
  {
  public:
    static const double MIN_DISTANCE_BETWEEN_UPDATES_KM;
  }; // _CONST

  explicit SpotsModel(QObject *parent = 0);
  ~SpotsModel();

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual QHash<int, QByteArray> roleNames() const;

public slots:

  void clear();

  void setUserId(int id);

  void getBy_Distance(const QGeoCoordinate &coordinate,
                      double maxDistance_km);

  void updateLocation(const QGeoCoordinate &coordinate);

signals:

  void countChanged(int count);

private slots:

  void slot_SpotRepository_DataReady(int requestId,
                                     bool success);

private:

  QList<Spot * > m_QList_Spots;
  int m_RequestId;

  QGeoCoordinate m_QGeoCoordinate_Location;
  double         m_MaxDistance_km;
};

#endif // SPOTSMODEL_H
