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

#include <QObject>

class SpotRepository : public QObject
{
  Q_OBJECT

public:

  explicit SpotRepository(QObject *parent = 0);
  ~SpotRepository();

signals:

public slots:
};

#endif // SPOTREPOSITORY_H
