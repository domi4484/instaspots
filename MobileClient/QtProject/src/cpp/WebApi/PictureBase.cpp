/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      03.11.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// File includes ---------------------------
#include "PictureBase.h"

// Project includes ------------------------
#include "../HelperClasses/Logger.h"
#include "WebApi.h"

//-----------------------------------------------------------------------------------------------------------------------------

PictureBase::PictureBase(QObject *parent) :
  QObject(parent),
  m_QMap_Pictures(),
  m_Command_GetNews()
{
  m_Command_GetNews.setAnswerType(WebApiCommand::JSON);
  m_Command_GetNews.setCommand(WebApi::C_GET_NEWS);
  connect(&m_Command_GetNews,
          SIGNAL(signal_Finished(const WebApiError &)),
          SLOT(slot_CommandGetNews_Finished(const WebApiError &)));
}

//-----------------------------------------------------------------------------------------------------------------------------

PictureBase::~PictureBase()
{
  while(m_QMap_Pictures.isEmpty() == false)
    delete m_QMap_Pictures.take(m_QMap_Pictures.keys().last());
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureBase::clear()
{
  m_QMap_Pictures.clear();
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureBase::execute()
{
  m_QMap_Pictures.clear();

  // TODO check post return type
  QList<QueryItem> qList_QueryItems;
  m_Command_GetNews.postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<int> PictureBase::getPicturesId()
{
  return m_QMap_Pictures.keys();
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<Picture *> PictureBase::getPictures()
{
  return m_QMap_Pictures.values();
}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureBase::slot_CommandGetNews_Finished(const WebApiError &error)
{
  QList<Picture *> newPictures;

  if(error.type() != WebApiError::NONE)
  {
    emit signal_News(error,
                     newPictures);
    return;
  }

  QScriptValue qScriptValue_Ids = m_Command_GetNews.resultProperty(WebApi::A_ARRAY_PICTURES);
  int length = qScriptValue_Ids.property("length").toInteger();

  for(int i = 0; i < length; i++)
  {
    int id = qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_ID).toInteger();

    if(m_QMap_Pictures.keys().contains(id) == false)
    {
      Picture *picture = new Picture(id,
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_ID_USER).toInteger(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_ID_SPOT).toInteger(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_URL).toString(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_USERNAME).toString(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_NAME).toString(),
                                     qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_DESCRIPTION).toString(),
                                     QDateTime::fromString(qScriptValue_Ids.property(i).property(WebApi::A_ARRAY_PICTURES_ELEMENT_CREATED).toString(),
                                                           Qt::ISODate));

      m_QMap_Pictures.insert(picture->id(), picture);
      newPictures.append(picture);
    }
  }

  emit signal_News(error,
                   newPictures);
}

//-----------------------------------------------------------------------------------------------------------------------------


