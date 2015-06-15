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

#ifndef NEARBYSPOTSMODEL_H
#define NEARBYSPOTSMODEL_H

// Qt includes -----------------------------
#include <QAbstractListModel>

// Forward declarations --------------------
class Picture;
class PictureRepository;

class PicturesModel : public QAbstractListModel
{
  Q_OBJECT

  public:

    explicit PicturesModel(PictureRepository *pictureRepository, QObject *parent = 0);
    ~PicturesModel();

    virtual int rowCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QHash<int, QByteArray> roleNames() const;

  signals:

  public slots:

    void setSpotId(int id);

  private slots:
    void slot_PictureRepository_DataReady();

  private:

    // Link to picture repository
    PictureRepository *m_PictureRepository;

    // News data
    QList<Picture * > m_QList_Pictures;
};

#endif // NEARBYSPOTSMODEL_H

