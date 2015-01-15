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

#ifndef NEWSMODEL_H
#define NEWSMODEL_H

// Projects includes -----------------------
#include "Picture.h"
#include "WebApiError.h"

// Qt includes -----------------------------
#include <QAbstractListModel>

// Forward declarations --------------------
class PictureBase;

class NewsModel : public QAbstractListModel
{
  Q_OBJECT
  public:

    explicit NewsModel(PictureBase *pictureBase,
                       QObject *parent = 0);
    ~NewsModel();

  virtual int rowCount(const QModelIndex &parent) const;

  virtual QVariant data(const QModelIndex &index, int role) const;

  virtual QHash<int, QByteArray> roleNames() const;

  signals:

  public slots:

    void getNewestSpots();

  private slots:
    void slot_News(const WebApiError &error,
                   const QList<Picture *> &pictures);
  private:

    // News data
    QList<Picture * > m_QList_Pictures;

    // Link to Picture Base
    PictureBase *m_PictureBase;

};

#endif // NEWSMODEL_H
