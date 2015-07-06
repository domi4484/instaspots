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

// Qt includes -----------------------------
#include <QAbstractListModel>

// Forward declarations --------------------
class Picture;

class NewsModel : public QAbstractListModel
{
   Q_OBJECT

   public:

   explicit NewsModel(QObject *parent = 0);
   ~NewsModel();

   virtual int rowCount(const QModelIndex &parent) const;

  virtual QVariant data(const QModelIndex &index, int role) const;

  virtual QHash<int, QByteArray> roleNames() const;

  signals:

  public slots:

    void getNewestSpots();

  private slots:
    void slot_PictureRepository_DataReady(int requestId, 
                                          bool success);

  private:

    // News data
    QList<Picture * > m_QList_Pictures;
    int m_RequestId;
};

#endif // NEWSMODEL_H
