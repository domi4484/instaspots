/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      29.10.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef QUERYITEM_H
#define QUERYITEM_H

// Qt includes -----------------------------
#include <QString>

class QueryItem
{
public:
  QueryItem();
  QueryItem(const QString &first,
            const QString &second);

  QString first()  const { return m_first;  }
  QString second() const { return m_second; }

private:
  QString m_first;
  QString m_second;
};

#endif // QUERYITEM_H
