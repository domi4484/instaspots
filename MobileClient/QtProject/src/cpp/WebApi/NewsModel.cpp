/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      18.12.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

// Files includes --------------------------
#include "NewsModel.h"

// Projects includes -----------------------
#include "Picture.h"
#include "PictureRepository.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

NewsModel::NewsModel(PictureRepository *pictureRepository,
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

NewsModel::~NewsModel()
{

}

//-----------------------------------------------------------------------------------------------------------------------------

int NewsModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return m_QList_Pictures.count();
}

//-----------------------------------------------------------------------------------------------------------------------------

QVariant NewsModel::data(const QModelIndex &index,
                         int role) const
{
  return m_QList_Pictures.at(index.row())->pictureRole((Picture::PictureRoles)role);
}

//-----------------------------------------------------------------------------------------------------------------------------

QHash<int, QByteArray> NewsModel::roleNames() const
{
  return Picture::roleNames();
}

//-----------------------------------------------------------------------------------------------------------------------------

void NewsModel::getNewestSpots()
{
  m_RequestId = m_PictureRepository->getNews();
}

//-----------------------------------------------------------------------------------------------------------------------------

void NewsModel::slot_PictureRepository_DataReady(int requestId,
                                                 bool success)
{
  if(m_RequestId != requestId)
    return;

  if(success == false)
    return;
  // TODO error handling?

  beginInsertRows(QModelIndex() , 0, m_PictureRepository->getPictures(m_RequestId).size()-1);
  m_QList_Pictures = m_PictureRepository->getPictures(m_RequestId);
  endInsertRows();
}

//-----------------------------------------------------------------------------------------------------------------------------
