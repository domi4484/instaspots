/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      07.04.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef APPLICATIONHELPER_H
#define APPLICATIONHELPER_H

// Qt includes -----------------------------
#include <QObject>

class ApplicationHelper : public QObject
{
  Q_OBJECT

public:
  explicit ApplicationHelper(QObject *parent = 0);
  ~ApplicationHelper();

signals:

  void signal_DevelopmentMode_Changed(bool developmentMode);

public slots:

  QString version();
  bool developmentMode() const { return m_DevelopmentMode; }

  void setDevelopmentMode(bool developmentMode);

private:

  bool m_DevelopmentMode;
};

#endif // APPLICATIONHELPER_H
