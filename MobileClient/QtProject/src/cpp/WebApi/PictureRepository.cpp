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

// Files includes --------------------------
#include "PictureRepository.h"

// Project includes ------------------------
#include "WebApi.h"

//-----------------------------------------------------------------------------------------------------------------------------

PictureRepository::PictureRepository(QObject *parent)
  : QObject(parent)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

PictureRepository::~PictureRepository()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

QList<Picture *> PictureRepository::getPictures()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void PictureRepository::getBy_SpotId(int spotId)
{
  QList<QueryItem> qList_QueryItems;
//  qList_QueryItems.append(QueryItem(WebApi::,  QString::number(latitude)));
//  qList_QueryItems.append(QueryItem(WebApi::R_PARAM_LONGITUDE, QString::number(longitude)));

  // TODO check post return type
//  m_Command_GetNearbySpots.postRequest(qList_QueryItems);
}

//-----------------------------------------------------------------------------------------------------------------------------

