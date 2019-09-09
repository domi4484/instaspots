#ifndef ENTITYPICTURE_H
#define ENTITYPICTURE_H

// Qt includes -----------------------------
#include <QObject>
#include <QDateTime>

class EntityPicture
{

public:

  EntityPicture();

  void Set_Id(qlonglong id);
  qlonglong Get_Id() const;

  void Set_UserId(qlonglong userId);
  qlonglong Get_UserId() const;

  void Set_SpotId(qlonglong spotId);
  qlonglong Get_SpotId() const;

  void Set_Created(const QDateTime qDateTime_Created);
  QDateTime Get_Created() const;

  void Set_Latitude(double latitude);
  double Get_Latitude() const;

  void Set_Longitude(double longitude);
  double Get_Longitude() const;

  void Set_Published(bool published);
  bool Get_Published() const;


  QString Get_Url() const;

  //-----------------------------------------------------------------------------------------------------------------------------

//    public function toJson()
//    {
//      $jPicture = array();
//      $jPicture[ParameterSet::PICTURE_PICTURE_ID] = $this->getId();
//      $jPicture[ParameterSet::PICTURE_LATITUDE  ] = $this->getLatitude();
//      $jPicture[ParameterSet::PICTURE_LONGITUDE ] = $this->getLongitude();
//      $jPicture[ParameterSet::PICTURE_URL       ] = $this->getUrl();
//      $jPicture[ParameterSet::PICTURE_CREATED   ] = $this->getCreatedISOText();

//      $jLikers = array();
//      foreach($this->getLikers() as &$liker)
//      {
//        $jLikers[] = $liker->toJson();
//      }
//      $jPicture[ParameterSet::PICTURE_LIKERS   ] = $jLikers;


//      $jPicture[ParameterSet::PICTURE_SPOT_ID         ] = $this->getSpot()->getId();
//      $jPicture[ParameterSet::PICTURE_SPOT_NAME       ] = $this->getSpot()->getName();
//      $jPicture[ParameterSet::PICTURE_SPOT_DESCRIPTION] = $this->getSpot()->getDescription();
//      $jPicture[ParameterSet::PICTURE_SPOT_SCORE      ] = $this->getSpot()->getScore();

//      $jPicture[ParameterSet::PICTURE_USER_ID      ] = $this->getUser()->getId();
//      $jPicture[ParameterSet::PICTURE_USER_USERNAME] = $this->getUser()->getUsername();

//      return $jPicture;
//    }


private:

  qlonglong m_Id;
  qlonglong m_UserId;
  qlonglong m_SpotId;
  QDateTime m_QDateTime_Created;
  double m_Latitude;
  double m_Longitude;
  bool m_Published;

};

#endif // ENTITYPICTURE_H
