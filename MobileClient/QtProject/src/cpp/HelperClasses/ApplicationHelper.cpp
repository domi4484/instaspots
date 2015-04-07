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

// Files includes --------------------------
#include "ApplicationHelper.h"

// Project includes ------------------------
#include "../WebApi/WebApi.h"

// Qt includes -----------------------------
#include <QDebug>
#include <QApplication>

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationHelper::ApplicationHelper(QObject *parent)
  : QObject(parent),
    m_DevelopmentMode(false)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

ApplicationHelper::~ApplicationHelper()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QString ApplicationHelper::version()
{
  return QApplication::applicationVersion();
}

//-----------------------------------------------------------------------------------------------------------------------------

void ApplicationHelper::setDevelopmentMode(bool developmentMode)
{
  m_DevelopmentMode = developmentMode;

  if(developmentMode)
  {
    WebApi::instance()->setUrl(WebApi::URL_DEVELOPMENT);
  }
  else
  {
    WebApi::instance()->setUrl(WebApi::URL_PRODUCTION);
  }

  emit signal_DevelopmentMode_Changed(developmentMode);
}

