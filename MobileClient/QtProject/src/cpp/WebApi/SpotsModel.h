/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      19.10.2015                                        *
 *                                                                 *
 * Copiright (c) 2015 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

#ifndef SPOTSMODEL_H
#define SPOTSMODEL_H


// Qt includes -----------------------------
#include <QAbstractListModel>

// Forward declarations --------------------
class Spot;

class SpotsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    explicit SpotsModel(QObject *parent = 0);
    ~SpotsModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

public slots:

    void clear();

    void setUserId(int id);

    void setLocation(double latitude,
                     double longitude,
                     double maxDistance_km);

signals:
    void countChanged(int count);

private slots:

    void slot_SpotRepository_DataReady(int requestId,
                                       bool success);

private:

    QList<Spot * > m_QList_Spots;
    int m_RequestId;
};

#endif // SPOTSMODEL_H
