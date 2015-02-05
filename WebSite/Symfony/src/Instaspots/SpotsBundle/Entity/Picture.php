<?php

namespace Instaspots\SpotsBundle\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Picture
 *
 * @ORM\Table(name="picture")
 * @ORM\Entity(repositoryClass="Instaspots\SpotsBundle\Entity\PictureRepository")
 */
class Picture
{
    /**
     * @ORM\ManyToOne(targetEntity="Instaspots\SpotsBundle\Entity\User")
     * @ORM\JoinColumn(nullable=false)
     */
    private $user;
    
    /**
     * @ORM\ManyToOne(targetEntity="Instaspots\SpotsBundle\Entity\Spot")
     * @ORM\JoinColumn(nullable=false)
     */
    private $spot;

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
     * @var bool
     *
     * @ORM\Column(name="published", type="boolean")
     */
    private $published = false;
     
     
     
    /**
     * Constructor
     */
    public function __construct()
    {
      $this->created = new \DateTime();
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
     * @return Picture
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
     * Set latitude
     *
     * @param float $latitude
     * @return Picture
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
     * @return Picture
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
     * Set published
     *
     * @param boolean $published
     * @return Picture
     */
    public function setPublished($published)
    {
        $this->published = $published;

        return $this;
    }

    /**
     * Get published
     *
     * @return boolean 
     */
    public function getPublished()
    {
        return $this->published;
    }

    /**
     * Set user
     *
     * @param \Instaspots\SpotsBundle\Entity\User $user
     * @return Picture
     */
    public function setUser(\Instaspots\SpotsBundle\Entity\User $user)
    {
        $this->user = $user;

        return $this;
    }

    /**
     * Get user
     *
     * @return \Instaspots\SpotsBundle\Entity\User 
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
}
