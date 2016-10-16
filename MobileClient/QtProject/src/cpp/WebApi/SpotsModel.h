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
#include <QGeoRectangle>
#include <QTimer>

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
    static const int    UPDATE_MODEL_DELAY_MS;
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
                      double maxDistance_m);

  void updateBy_Location(const QGeoCoordinate &qGeoCoordinate);
  void updateBy_VisibleRegion(const QGeoRectangle &qGeoRectangle);

signals:

  void countChanged(int count);

private slots:

  void slot_SpotRepository_DataReady(int requestId,
                                     bool success);
  void slot_QTimer_UpdateModel_timeout();

private:

  QList<Spot * > m_QList_Spots;
  int m_RequestId;

  QGeoRectangle m_QGeoRectangle_VisibleRegion;

  QTimer m_QTimer_UpdateModel;
};

#endif // SPOTSMODEL_H
