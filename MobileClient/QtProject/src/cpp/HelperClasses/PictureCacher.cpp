/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      19.03.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "PictureCacher.h"

// Qt includes -----------------------------
#include <QRegularExpression>

//-----------------------------------------------------------------------------------------------------------------------------

PictureCacher::PictureCacher(QObject *parent)
  : QObject(parent),
    m_PictureUrl_Picture()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

PictureCacher::~PictureCacher()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

bool PictureCacher::isPictureCached(const QString &url)
{
  return m_PictureUrl_Picture.contains(url);
}

//-----------------------------------------------------------------------------------------------------------------------------

QString PictureCacher::getThumb(const QString &url)
{
  // No source set
  if(url.isEmpty())
  {
    return url;
  }

  QString sourceUrl = url;
  QRegularExpression regex(".jpg$");
  return sourceUrl.replace(regex, "-thumb.jpg");
}

//-----------------------------------------------------------------------------------------------------------------------------





