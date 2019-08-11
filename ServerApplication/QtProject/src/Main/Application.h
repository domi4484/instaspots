/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      15.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2016 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

// Qt includes -----------------------------
#include <QCoreApplication>
#include <QUrl>

class Application : public QCoreApplication
{
    Q_OBJECT
public:

  class _CONST
  {
  public:
    class SETTINGS
    {
    public:
      static const QString DIRECTORY;
    }; // SETTINGS
  }; // _CONST

  explicit Application(int argc,
                       char *argv[]);
  ~Application();

private:

  void startupApplication_Settings();
};

#endif // APPLICATION_H
