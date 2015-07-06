/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.06.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef SPOTREPOSITORY_H
#define SPOTREPOSITORY_H

// Project includes ------------------------
#include "WebApiError.h"

// Qt includes -----------------------------
#include <QObject>
#include <QMap>

// Forward declarations --------------------
class Spot;

class SpotRepository : public QObject
{

  Q_OBJECT

public:

  explicit SpotRepository(QObject *parent = 0);
  ~SpotRepository();

  static void instanziate();
  static void destroy();

  static SpotRepository *instance() { return s_SpotRepository; }

public slots:

  QList<Spot *> getSpots(int requestId);

  int getByDistance(double latitude,
                    double longitude,
                    double maxDistance_km);

signals:

  void signal_DataReady(int requestId,
                        bool success);

private slots:

  void slot_Command_Finished(const WebApiError &error);

private:

  static const char* PROPERTY_REQUEST_ID;

  static SpotRepository *s_SpotRepository;

  int m_RequestId;

  QMap<int, Spot *> m_QMap_Spots;

  QMap<int, QList<Spot *> > m_QMap_Results;
};

#endif // SPOTREPOSITORY_H
