<?php

namespace Instaspots\UserBundle\Entity;

// Project imports -------------------------
use Instaspots\SpotsBundle\Controller\ParameterSet;

// Doctrine imports ------------------------
use Doctrine\ORM\Mapping as ORM;

use FOS\UserBundle\Model\User as BaseUser;

/**
 * User
 *
 * @ORM\Entity
 */
class User extends BaseUser
{
    /**
     * @var integer
     *
     * @ORM\Column(name="id", type="integer")
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="AUTO")
     */
    protected $id;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="created", type="datetime")
     */
    private $created;

    /**
     * @var integer
     *
     * @ORM\Column(name="reputation", type="integer")
     */
    private $reputation = 0;


    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct();

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
     * @return User
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
     * Set reputation
     *
     * @param integer $reputation
     * @return User
     */
    public function setReputation($reputation)
    {
        $this->reputation = $reputation;

        return $this;
    }

    /**
     * Get reputation
     *
     * @return integer 
     */
    public function getReputation()
    {
        return $this->reputation;
    }

    public function addReputation($reputation)
    {
        $this->setReputation($this->reputation + $reputation);

        return $this;
    }

    public function removeReputation($reputation)
    {
        $this->setReputation($this->reputation - $reputation);

        if($this->reputation < 0)
          $this->setReputation(0);

        return $this;
    }

//-----------------------------------------------------------------------------------------------------------------------------

  public function toJson()
  {
    $jUser = array();
    $jUser[ParameterSet::USER_USER_ID ] = $this->getId();
    $jUser[ParameterSet::USER_USERNAME] = $this->getUsername();

    return $jUser;
  }

  public function notifyLikedPicture(\Instaspots\SpotsBundle\Entity\Picture $pictureLiked)
  {
    $this->addReputation(1);
  }
}
