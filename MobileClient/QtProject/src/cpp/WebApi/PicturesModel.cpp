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
#include "PicturesModel.h"

// Projects includes -----------------------
#include "Picture.h"
#include "PictureRepository.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

PicturesModel::PicturesModel(PictureRepository *pictureRepository,
                             QObject *parent)
  : QAbstractListModel(parent),
    m_PictureRepository(pictureRepository),
    m_QList_Pictures(),
    m_RequestId(0)
{
  connect(m_PictureRepository,
          SIGNAL(signal_DataReady(int,
                                  bool)),
          SLOT(slot_PictureRepository_DataReady(int,
                                                bool)));
}

//-----------------------------------------------------------------------------------------------------------------------------

PicturesModel::~PicturesModel()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

int PicturesModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_QList_Pictures.count();
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant PicturesModel::data(const QModelIndex &index,
                             int role) const
{
  return m_QList_Pictures.at(index.row())->pictureRole((Picture::PictureRoles)role);
}

//-----------------------------------------------------------------------------------------------------------------------------

QHash<int, QByteArray> PicturesModel::roleNames() const
{
  return Picture::roleNames();
}

//-----------------------------------------------------------------------------------------------------------------------------

void PicturesModel::setSpotId(int id)
{
  qDebug() << "setSpotId(int id)" << id;
  m_RequestId = m_PictureRepository->getBy_SpotId(id);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PicturesModel::slot_PictureRepository_DataReady(int requestId,
                                                     bool success)
{
  if(m_RequestId != requestId)
    return;

  if(success == false)
    return;
  // TODO error handling?

  m_QList_Pictures = m_PictureRepository->getPictures(m_RequestId);
}

//-----------------------------------------------------------------------------------------------------------------------------
