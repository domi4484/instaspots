/********************************************************************
 *                                                                 *
 * Lowerspot                                                       *
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
#include "../HelperClasses/Logger.h"
#include "Picture.h"
#include "PictureRepository.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

PicturesModel::PicturesModel(QObject *parent)
  : QAbstractListModel(parent),
    m_QList_Pictures(),
    m_RequestId(0)
{
    m_RequestId = PictureRepository::instance()->getNewRequestId();

    connect(PictureRepository::instance(),
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

Picture *PicturesModel::first() const
{
  if(m_QList_Pictures.isEmpty())
    return NULL;

  return m_QList_Pictures.first();
}

//-----------------------------------------------------------------------------------------------------------------------------

Picture *PicturesModel::getPicture(int index) const
{
  if(index >= m_QList_Pictures.size())
    return NULL;

  return m_QList_Pictures.at(index);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PicturesModel::getNewestSpots()
{
    PictureRepository::instance()->getBy_Newest(m_RequestId);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PicturesModel::getBy_PictureId(int id)
{
    PictureRepository::instance()->getBy_PictureId(m_RequestId,
                                                   id);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PicturesModel::getBy_SpotId(int id)
{

    PictureRepository::instance()->getBy_SpotId(m_RequestId,
                                                id);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PicturesModel::getBy_UserId(int id)
{
    PictureRepository::instance()->getBy_UserId(m_RequestId,
                                                id);
}

//-----------------------------------------------------------------------------------------------------------------------------

void PicturesModel::slot_PictureRepository_DataReady(int requestId,
                                                     bool success)
{
  if(m_RequestId != requestId)
    return;

  if(success == false)
  {
    Logger::error(QString("slot_PictureRepository_DataReady: success = %1").arg(success));
    return;
  }

  if(m_QList_Pictures != PictureRepository::instance()->getPictures(m_RequestId))
  {
  QAbstractItemModel::beginResetModel();
  m_QList_Pictures.clear();
  QAbstractItemModel::endResetModel();

  int newCount = PictureRepository::instance()->getPictures(m_RequestId).size();
  if(newCount > 0)
  {
    QAbstractItemModel::beginInsertRows(QModelIndex() , 0, PictureRepository::instance()->getPictures(m_RequestId).size()-1);
    m_QList_Pictures = PictureRepository::instance()->getPictures(m_RequestId);
    QAbstractItemModel::endInsertRows();
  }

  countChanged(m_QList_Pictures.count());
  }
}

//-----------------------------------------------------------------------------------------------------------------------------
