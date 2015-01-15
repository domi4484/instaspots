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
#include "PictureBase.h"

// Qt includes -----------------------------
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

NewsModel::NewsModel(PictureBase *pictureBase,
                     QObject *parent)
  : QAbstractListModel(parent),
    m_PictureBase(pictureBase)
{
  connect(m_PictureBase,
          SIGNAL(signal_News(const WebApiError &,
                             const QList<Picture*> &)),
          SLOT(slot_News(const WebApiError &,
                         const QList<Picture*> &)));
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
  m_PictureBase->execute();
}

//-----------------------------------------------------------------------------------------------------------------------------

void NewsModel::slot_News(const WebApiError &error,
                          const QList<Picture *> &pictures)
{
  if(error.type() != WebApiError::NONE)
    return;

  beginInsertRows(QModelIndex() , 0, pictures.size()-1);
  m_QList_Pictures.append(pictures);
  endInsertRows();
}

//-----------------------------------------------------------------------------------------------------------------------------
