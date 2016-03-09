<?php

namespace Instaspots\SpotsBundle\Entity;

// Project imports -------------------------
use Instaspots\SpotsBundle\Controller\ParameterSet;

// Doctrine imports ------------------------
use Doctrine\ORM\Mapping as ORM;

/**
 * Spot
 *
 * @ORM\Table()
 * @ORM\Entity(repositoryClass="Instaspots\SpotsBundle\Entity\SpotRepository")
 */
class Spot
{
    /**
     * @ORM\ManyToOne(targetEntity="Instaspots\UserBundle\Entity\User")
     * @ORM\JoinColumn(nullable=false)
     */
    private $user;

    /**
     * @var integer
     *
     * @ORM\Column(name="id", type="integer")
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="AUTO")
     */
    private $id;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="created", type="datetime")
     */
    private $created;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="modified", type="datetime")
     */
    private $modified;

    /**
     * @var string
     *
     * @ORM\Column(name="name", type="string", length=255)
     */
    private $name;

    /**
     * @var string
     *
     * @ORM\Column(name="description", type="text")
     */
    private $description;

    /**
     * @var boolean
     *
     * @ORM\Column(name="secretSpot", type="boolean")
     */
    private $secretSpot;

    /**
     * @var integer
     *
     * @ORM\Column(name="score", type="integer")
     */
    private $score;

    /**
     * @var float
     *
     * @ORM\Column(name="latitude", type="float")
     */
    private $latitude;

    /**
     * @var float
     *
     * @ORM\Column(name="longitude", type="float")
     */
    private $longitude;

    /**
     * @ORM\OneToOne(targetEntity="Instaspots\SpotsBundle\Entity\Picture")
     */
    private $picture1;

    /**
     * @ORM\OneToOne(targetEntity="Instaspots\SpotsBundle\Entity\Picture")
     */
    private $picture2;


    /**
     * @ORM\OneToMany(targetEntity="Instaspots\SpotsBundle\Entity\Picture", mappedBy="spot")
     */
    private $pictures; // // Inverso della relazione many to one


    /**
     * @var float
     */
    private $distance;


    /**
     * Constructor
     */
    public function __construct()
    {
      $now = new \DateTime();
      $this->created = $now;
      $this->modified = $now;

      $this->score = 0;
    }


    /**
     * Get id
     *
     * @return integer
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * Set created
     *
     * @param \DateTime $created
     * @return Spot
     */
    public function setCreated($created)
    {
        $this->created = $created;

        return $this;
    }

    /**
     * Get created
     *
     * @return \DateTime
     */
    public function getCreated()
    {
        return $this->created;
    }

    /**
     * Set modified
     *
     * @param \DateTime $modified
     * @return Spot
     */
    public function setModified($modified)
    {
        $this->modified = $modified;

        return $this;
    }

    /**
     * Get modified
     *
     * @return \DateTime
     */
    public function getModified()
    {
        return $this->modified;
    }

    /**
     * Set name
     *
     * @param string $name
     * @return Spot
     */
    public function setName($name)
    {
        $this->name = $name;

        return $this;
    }

    /**
     * Get name
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * Set description
     *
     * @param string $description
     * @return Spot
     */
    public function setDescription($description)
    {
        $this->description = $description;

        return $this;
    }

    /**
     * Get description
     *
     * @return string
     */
    public function getDescription()
    {
        return $this->description;
    }

    /**
     * Set secretSpot
     *
     * @param bool $secretSpot
     * @return Spot
     */
    public function setSecretSpot($secretSpot)
    {
        $this->secretSpot = $secretSpot;

        return $this;
    }

    /**
     * Get secretSpot
     *
     * @return boolean
     */
    public function getSecretSpot()
    {
        return $this->secretSpot;
    }

    /**
     * Set score
     *
     * @param integer $score
     * @return Spot
     */
    public function setScore($score)
    {
        $this->score = $score;

        return $this;
    }

    /**
     * Get score
     *
     * @return integer
     */
    public function getScore()
    {
        return $this->score;
    }

    /**
     * Set user
     *
     * @param \Instaspots\UserBundle\Entity\User $user
     * @return Spot
     */
    public function setUser(\Instaspots\UserBundle\Entity\User $user)
    {
        $this->user = $user;

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
     * Set latitude
     *
     * @param float $latitude
     * @return Spot
     */
    public function setLatitude($latitude)
    {
        $this->latitude = $latitude;

        return $this;
    }

    /**
     * Get latitude
     *
     * @return float
     */
    public function getLatitude()
    {
        return $this->latitude;
    }

    /**
     * Set longitude
     *
     * @param float $longitude
     * @return Spot
     */
    public function setLongitude($longitude)
    {
        $this->longitude = $longitude;

        return $this;
    }

    /**
     * Get longitude
     *
     * @return float
     */
    public function getLongitude()
    {
        return $this->longitude;
    }

    /**
     * Set picture1
     *
     * @param \Instaspots\SpotsBundle\Entity\Picture $picture1
     * @return Spot
     */
    public function setPicture1(\Instaspots\SpotsBundle\Entity\Picture $picture1 = null)
    {
        $this->picture1 = $picture1;

        return $this;
    }

    /**
     * Get picture1
     *
     * @return \Instaspots\SpotsBundle\Entity\Picture
     */
    public function getPicture1()
    {
        return $this->picture1;
    }

    /**
     * Set picture2
     *
     * @param \Instaspots\SpotsBundle\Entity\Picture $picture2
     * @return Spot
     */
    public function setPicture2(\Instaspots\SpotsBundle\Entity\Picture $picture2 = null)
    {
        $this->picture2 = $picture2;

        return $this;
    }

    /**
     * Get picture2
     *
     * @return \Instaspots\SpotsBundle\Entity\Picture
     */
    public function getPicture2()
    {
        return $this->picture2;
    }

    //-----------------------------------------------------------------------------------------------------------------------------

    public function pictureAdded(\Instaspots\SpotsBundle\Entity\Picture $picture)
    {


    }

    //-----------------------------------------------------------------------------------------------------------------------------

    public function addPicture(Picture $picture)
    {
      $this->pictures[] = $picture;
      $picture->setSpot($this);

      $picturesCount = count($this->pictures);


      if($picturesCount == 1) // First picture added
      {
        $this->latitude  = $picture->getLatitude();
        $this->longitude = $picture->getLongitude();
        $this->picture1 = $picture;
        $this->picture2 = $picture;
        return $this;
      }


      // Update location
      $this->latitude  = ($this->latitude  * ($picturesCount - 1) + $picture->getLatitude() ) / $picturesCount;
      $this->longitude = ($this->longitude * ($picturesCount - 1) + $picture->getLongitude()) / $picturesCount;

      // Check if picture1 and 2 are still the same
      if($this->picture1->getId() == $this->picture2->getId())
      {
        $this->picture1 = $picture;
        return $this;
      }

      if($this->picture1->getLikers()->count() == 0)
      {
        $this->picture2 = $this->picture1;
        $this->picture1 = $picture;
        return $this;
      }

      if($this->picture2->getLikers()->count() == 0)
      {
        $this->setPicture2($picture);
        return $this;
      }

      return $this;
    }

    public function removePicture(Picture $picture)
    {
      $this->pictures->removeElement($picture);
    }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function getPictures()
  {
    return $this->pictures;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function setDistance($distance = -1)
  {
      $this->distance = $distance;

      return $this;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function getDistance()
  {
      return $this->distance;
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  public function toJson()
  {
    $jSpot = array();

    $jSpot[ParameterSet::SPOT_SPOT_ID    ] = $this->getId();
    $jSpot[ParameterSet::SPOT_NAME       ] = $this->getName();
    $jSpot[ParameterSet::SPOT_DESCRIPTION] = $this->getDescription();
    $jSpot[ParameterSet::SPOT_SECRET_SPOT] = $this->getSecretSpot();
    $jSpot[ParameterSet::SPOT_LATITUDE   ] = $this->getLatitude();
    $jSpot[ParameterSet::SPOT_LONGITUDE  ] = $this->getLongitude();
    $jSpot[ParameterSet::SPOT_DISTANCE_KM] = -1;

    $picture1 = $this->getPicture1();
    $jSpot[ParameterSet::SPOT_PICTURE_PICTURE_ID_1] = $picture1->getId();
    $jSpot[ParameterSet::SPOT_PICTURE_URL_1       ] = $picture1->getUrl();

    $picture2 = $this->getPicture2();
    if($picture2->getId() != $picture1->getId())
    {
      $jSpot[ParameterSet::SPOT_PICTURE_PICTURE_ID_2] = $picture2->getId();
      $jSpot[ParameterSet::SPOT_PICTURE_URL_2       ] = $picture2->getUrl();
    }
    else
    {
      $jSpot[ParameterSet::SPOT_PICTURE_PICTURE_ID_2]  = -1;
      $jSpot[ParameterSet::SPOT_PICTURE_URL_2       ] = '';
    }

    return $jSpot;
  }
}
