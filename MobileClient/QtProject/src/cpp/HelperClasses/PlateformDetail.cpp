/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      11.01.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "PlateformDetail.h"

//-----------------------------------------------------------------------------------------------------------------------------

PlateformDetail::PlateformDetail(QObject *parent) :
  QObject(parent),
  m_Os(OS_OTHER)
{
#ifdef Q_OS_ANDROID
  m_Os = OS_ANDROID;
#endif
#ifdef Q_OS_IOS
  m_Os = OS_IOS;
#endif
#ifdef Q_OS_WINPHONE
  m_Os = OS_WINPHONE;
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------

PlateformDetail::~PlateformDetail()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

PlateformDetail::OS PlateformDetail::getOS() const
{
  return m_Os;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PlateformDetail::isMobile()
{
  return m_Os != OS_OTHER;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool PlateformDetail::isIOS()
{
  return m_Os == OS_IOS;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString PlateformDetail::name()
{
  switch (m_Os) {
  case OS_ANDROID:
    return "Android";
  break;
  case OS_IOS:
    return "iOS";
  break;
  case OS_WINPHONE:
    return "Windows phone";
  break;
  case OS_OTHER:
    return "Desktop";
  break;
  }

  return QString();
}

