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

#ifndef PICTURESMODEL_H
#define PICTURESMODEL_H

// Qt includes -----------------------------
#include <QAbstractListModel>

// Forward declarations --------------------
class Picture;

class PicturesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:

    explicit PicturesModel(QObject *parent = 0);
    ~PicturesModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

  public slots:

    Picture *first() const;

    Picture *getPicture(int index) const;

    void getNewestSpots();

    void getBy_PictureId(int id);
    void getBy_SpotId(int id);
    void getBy_UserId(int id);

signals:
    void countChanged(int count);

private slots:
    void slot_PictureRepository_DataReady(int requestId,
                                          bool success);

private:

    // News data
    QList<Picture * > m_QList_Pictures;
    int m_RequestId;
};

#endif // PICTURESMODEL_H
