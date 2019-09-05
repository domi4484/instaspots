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

  void Set_SpotId(qlonglong userId);
  qlonglong Get_SpotId() const;

  void Set_Created(const QDateTime qDateTime_Created);
  QDateTime Get_Created() const;

  void Set_Latitude(double latitude);
  double Get_Latitude() const;

  void Set_Longitude(double longitude);
  double Get_Longitude() const;

  void Set_Published(bool published);
  bool Get_Published() const;


      /**
       * Set user
       *
       * @param \Instaspots\UserBundle\Entity\User $user
       * @return Picture
       */
      public function setUser(\Instaspots\UserBundle\Entity\User $user)
      {
          $this->user = $user;

          // Add score to user
          $user->addReputation(10);

          return $this;
      }

      /**
       * Get user
       *
       * @return \Instaspots\UserBundle\Entity\User
       */
      public function getUser()
      {
          return $this->user;
      }

      /**
       * Set spot
       *
       * @param \Instaspots\SpotsBundle\Entity\Spot $spot
       * @return Picture
       */
      public function setSpot(\Instaspots\SpotsBundle\Entity\Spot $spot)
      {
          $this->spot = $spot;

          return $this;
      }

      /**
       * Get spot
       *
       * @return \Instaspots\SpotsBundle\Entity\Spot
       */
      public function getSpot()
      {
          return $this->spot;
      }

      public function getUrl()
      {
        return sprintf('https://lowerspot.com/web/pictures/%s/%s.jpg',
                       $this->created->format('Y/m/d'),
                       $this->id);
      }

      /**
       * Add likers
       *
       * @param \Instaspots\UserBundle\Entity\User $likers
       * @return Picture
       */
      public function addLiker(\Instaspots\UserBundle\Entity\User $liker)
      {
        // Check if already liked
        if($this->likers->contains($liker))
          return $this;

        // Assign reputation point to liked user
        if($this->user != $liker)
        {
          $this->user->notifyLikedPicture($this);
        }

        // Assign score point to spot
        $this->spot->addScore(1);

        // Append liker
        $this->likers[] = $liker;

        return $this;
      }

      /**
       * Remove likers
       *
       * @param \Instaspots\UserBundle\Entity\User $likers
       */
      public function removeLiker(\Instaspots\UserBundle\Entity\User $liker)
      {
        // Check if already unliked
        if($this->likers->contains($liker) == false)
          return $this;

        // Remove reputation point to liked user
        if($this->user != $liker)
        {
          $this->user->removeReputation(1);
        }

        // Remove score point from spot
        $this->spot->removeScore(1);

        // Remove liker
        $this->likers->removeElement($liker);
      }

      /**
       * Get likers
       *
       * @return \Doctrine\Common\Collections\Collection
       */
      public function getLikers()
      {
          return $this->likers;
      }

      //-----------------------------------------------------------------------------------------------------------------------------

    public function toJson()
    {
      $jPicture = array();
      $jPicture[ParameterSet::PICTURE_PICTURE_ID] = $this->getId();
      $jPicture[ParameterSet::PICTURE_LATITUDE  ] = $this->getLatitude();
      $jPicture[ParameterSet::PICTURE_LONGITUDE ] = $this->getLongitude();
      $jPicture[ParameterSet::PICTURE_URL       ] = $this->getUrl();
      $jPicture[ParameterSet::PICTURE_CREATED   ] = $this->getCreatedISOText();

      $jLikers = array();
      foreach($this->getLikers() as &$liker)
      {
        $jLikers[] = $liker->toJson();
      }
      $jPicture[ParameterSet::PICTURE_LIKERS   ] = $jLikers;


      $jPicture[ParameterSet::PICTURE_SPOT_ID         ] = $this->getSpot()->getId();
      $jPicture[ParameterSet::PICTURE_SPOT_NAME       ] = $this->getSpot()->getName();
      $jPicture[ParameterSet::PICTURE_SPOT_DESCRIPTION] = $this->getSpot()->getDescription();
      $jPicture[ParameterSet::PICTURE_SPOT_SCORE      ] = $this->getSpot()->getScore();

      $jPicture[ParameterSet::PICTURE_USER_ID      ] = $this->getUser()->getId();
      $jPicture[ParameterSet::PICTURE_USER_USERNAME] = $this->getUser()->getUsername();

      return $jPicture;
    }


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
