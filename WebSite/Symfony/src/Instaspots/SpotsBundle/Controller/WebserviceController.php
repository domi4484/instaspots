<?php

namespace Instaspots\SpotsBundle\Controller;

// Bundle imports --------------------------
use Instaspots\UserBundle\Entity\User;
use Instaspots\SpotsBundle\Entity\Spot;
use Instaspots\SpotsBundle\Entity\Picture;

// Project imports -------------------------
use Instaspots\SpotsBundle\Controller\ApplicationHelper;
use Instaspots\SpotsBundle\Controller\CommandSet;
use Instaspots\SpotsBundle\Controller\ParameterSet;
use Instaspots\SpotsBundle\Controller\Response;

// Symfony imports -------------------------
use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\FileBag;
use Symfony\Component\HttpFoundation\File\UploadedFile;

use Symfony\Component\Filesystem\Filesystem;
use Symfony\Component\Filesystem\Exception\IOExceptionInterface;

use Symfony\Component\Security\Http\Event\InteractiveLoginEvent;
use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;

// Doctrine imports ------------------------
use Doctrine\DBAL\DBALException;

// FOS imports -----------------------------
use FOS\UserBundle\Util\UserManipulator;

// Php imports -----------------------------
use SoapClient;
use SoapFault;

class WebserviceController extends Controller
{
  private $CONST_DEBUG = false;

  public function webserviceAction(Request $request)
  {
    $response = new Response();

    if (   $request->isMethod('POST') == false
        && $this->CONST_DEBUG         == false)
    {
      $response->setError('Not a post request');
      return new JsonResponse($response->toJson());
    }

    $response->setCommand       ($request->get('command'));
    $response->setClientVersion ($request->get('version'));

    if (strlen($response->getCommand()) == 0)
    {
      $response->setError('Empty command');
      return new JsonResponse($response->toJson());
    }

    if (strlen($response->getClientVersion()) == 0)
    {
      $response->setClientVersion('V0.0.0');
      $response->setWarning('Empty version');
    }

    switch ($response->getCommand())
    {
      case CommandSet::LOGIN:
        $this->login($request, $response);
      break;

      case CommandSet::LOGOUT:
        $this->logout();
      break;

      case CommandSet::CANREGISTER:
        $this->canregister($request, $response);
      break;

      case CommandSet::REGISTER:
        $this->register($request, $response);
      break;

      case CommandSet::REPORT_PROBLEM:
        $this->reportProblem($request, $response);
      break;

      case CommandSet::GET_CURRENT_CLIENT_VERSION:
        $this->getCurrentClientVersion($response);
      break;

      case CommandSet::UPLOAD_PICTURE_TO_SPOT:
        $this->uploadPictureToSpot($request, $response);
      break;

      case CommandSet::UPLOAD_NEW_SPOT:
        $this->uploadNewSpot($request, $response);
      break;

      case CommandSet::GET_PICTURES_BY_NEWEST:
        $this->getPicturesByNewest($response);
      break;

      case CommandSet::GET_PICTURES_BY_SPOT_ID:
        $this->getPicturesBySpotId($request, $response);
      break;

      case CommandSet::GET_PICTURES_BY_USER_ID:
        $this->getPicturesByUserId($request, $response);
      break;

      case CommandSet::GET_SPOTS_BY_DISTANCE:
        $this->getSpotsByDistance($request, $response);
      break;

      case CommandSet::GET_SPOTS_BY_USER_ID:
        $this->getSpotsByUserId($request, $response);
      break;

      case CommandSet::SPOT_SIGNAL_DISAPPEARED:
        $this->spotSignalDisappeared($request, $response);
      break;

      case CommandSet::PICTURE_LIKE:
        $this->pictureLike($request, $response);
      break;

      case CommandSet::PICTURE_UNLIKE:
        $this->pictureUnlike($request, $response);
      break;

      case CommandSet::PICTURE_REMOVE:
        $this->pictureRemove($request, $response);
      break;

      default:
      {
        $response->setError("Unknown command: ".$response->getCommand());
      }
    }

    return new JsonResponse($response->toJson());
  }

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

  /**
   * Login
   *
   * @param \Request  $request
   * @param \Response $response
   * @return WebserviceController
   */
  private function login($request, &$response)
  {
    $minimumClientVersion = 'V0.0.1';

    $username = $request->get(ParameterSet::USER_USERNAME);
    $password = $request->get(ParameterSet::USER_PASSWORD);

    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsUserBundle:User');

    $listUsers = $repository->findBy(array('username' => $username));

    if(empty($listUsers))
    {
      $response->addData(ParameterSet::USER_AUTHENTICATED, false);
      return;
    }
    $user = current($listUsers);

    $factory = $this->get('security.encoder_factory');
    $encoder = $factory->getEncoder($user);

    // Creo il token
    $token = new UsernamePasswordToken($user, null, "your_firewall_name", $user->getRoles());
    $this->get("security.context")->setToken($token); //now the user is logged in

    //now dispatch the login event
    $request = $this->get("request");
    $event = new InteractiveLoginEvent($request, $token);
    $this->get("event_dispatcher")->dispatch("security.interactive_login", $event);

    $validPassword = $encoder->isPasswordValid($user->getPassword(),
                                               $password,
                                               $user->getSalt());

    $response->addData(ParameterSet::USER_USER_ID,       $user->getId());
    $response->addData(ParameterSet::USER_AUTHENTICATED, $validPassword);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function logout()
  {
    $minimumClientVersion = 'V0.0.1';

  // TODO
    //$_SESSION = array();
    //session_destroy();
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function canregister($request, &$response )
  {
    $minimumClientVersion = 'V0.0.1';

    // Get parameters
    $username = $request->get(ParameterSet::USER_USERNAME);

    // check if username is not empty
    if (strlen($username) == 0)
    {
      $response->setError('Empty username');
      return;
    }

    $repository = $this->getDoctrine()
                       ->getManager()
                       ->getRepository('InstaspotsUserBundle:User');

    $listUsers = $repository->findByUsername($username);

    if(empty($listUsers) == false)
    {
      $response->addData(ParameterSet::USER_REGISTERED, true);
      return;
    }

    $response->addData(ParameterSet::USER_REGISTERED, false);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function register($request, &$response)
  {
    $minimumClientVersion = 'V0.0.1';

    // Get parameters
    $username = $request->get(ParameterSet::USER_USERNAME);
    $password = $request->get(ParameterSet::USER_PASSWORD);
    $email    = $request->get(ParameterSet::USER_EMAIL);

    // check if username is not empty
    if (strlen($username) == 0)
    {
      $response->setError('Empty username');
      return;
    }

    // check if password is not empty
    if (strlen($password) == 0)
    {
      $response->setError('Invalid password');
      return;
    }

    // validate email
    if (strlen($email) == 0)
    {
      $response->setError('Empty email');
      return;
    }

    if (filter_var($email, FILTER_VALIDATE_EMAIL) === false)
    {
      $response->setError('Invalid email format');
      return;
    }

    // check if user was already registered
    $this->canregister($request, $response);

    if($response->getData(ParameterSet::USER_REGISTERED) == true)
    {
      $response->addData(ParameterSet::USER_REGISTERED, false);
      return;
    }

    // Create new user
    $manipulator = $this->container->get('fos_user.util.user_manipulator');

    $active = true;
    $superadmin = false;
    $manipulator->create($username,
                         $password,
                         $email,
                         $active,
                         $superadmin);

    // Login the new user
    $this->login($request, $response);

    $response->addData(ParameterSet::USER_REGISTERED, true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function reportProblem($request, &$response)
  {
    $minimumClientVersion = 'V0.0.4';

    // Get parameters
    $reportTitle   = $request->get(ParameterSet::REPORT_TITLE);
    $reportContent = $request->get(ParameterSet::REPORT_CONTENT);

    // Create new bug report in mantis database
    $soapClient = new SoapClient("http://mantis.lowerclassclothing.com/api/soap/mantisconnect.php?wsdl");
    $username = 'client';
    $password = '4qpSLQxy';
    $issue = array (
                    'category'    =>"General",
                    'summary'     => "### ".$reportTitle,
                    'description' => $reportTitle."\n\n### Report content ###\n".$reportContent,
                    'project'     =>array('id'=>1)
                    );

    try
    {
      $soapClient->mc_issue_add($username, $password, $issue);
    }
    catch (SoapFault $exception)
    {
      $response->setError($exception->getMessage());
      return;
    }
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getCurrentClientVersion(&$response)
  {
    $response->addData(ParameterSet::APPLICATION_VERSION, 'V0.0.8');
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function uploadPictureToSpot($request, &$response)
  {
    $minimumClientVersion = 'V0.0.1';

    // Get parameters
    $spotId       = $request->get(ParameterSet::PICTURE_SPOT_ID);
    $latitude     = $request->get(ParameterSet::PICTURE_LATITUDE);
    $longitude    = $request->get(ParameterSet::PICTURE_LONGITUDE);
    $uploadedFile = $request->files->get('image');

    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response->setError('Authentication required');
      return;
    }

    // Check location
    if(   $latitude  == 0
       || $longitude == 0)
    {
      $response->setError('Invalid location');
      return;
    }

    // Check for valid picture
    if(   $uploadedFile == null
       || $uploadedFile->isValid() == false
       || $uploadedFile->getMimeType() != 'image/jpeg')
    {
      $response->setError('Invalid file received');
      return;
    }

    // Get spot
    $em = $this->getDoctrine()->getManager();
    $spotRepository = $em->getRepository('InstaspotsSpotsBundle:Spot');

    $spot = $spotRepository->findOneById($spotId);

    // New picture
    $picture = new Picture();
    $picture->setUser($user);
    $picture->setLatitude($latitude);
    $picture->setLongitude($longitude);
    $picture->setPublished(true);

    $spot->addPicture($picture);

    $em->persist($picture);

    // Create the directory for the new pictures
    $destinationDirectory = 'pictures/'.$picture->getCreated()->format('Y/m/d/');
    $fs = new Filesystem();
    try
    {
      $fs->mkdir($destinationDirectory);
    }
    catch (IOExceptionInterface $e)
    {
      $response->setError("An error occurred while creating directory at ".$e->getPath());
      return;
    }

    // Flush
    try
    {
      $em->flush();
    }
    catch (DBALException $e)
    {
      $response->setError("Flush exception ".$e->getMessage());
      return;
    }

    // Move the temporarily stored file to a convenient location
    $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');

    // Generate thumbnail
    $this->thumb($movedFile->getPathname(), 180);

    $response->addData('successful', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function uploadNewSpot($request, &$response)
  {
    $minimumClientVersion = 'V0.0.3';

    // Get parameters
    $name         = $request->get(ParameterSet::SPOT_NAME);
    $description  = $request->get(ParameterSet::SPOT_DESCRIPTION);
    $secretSpot   = $request->get(ParameterSet::SPOT_SECRET_SPOT);
    $latitude     = $request->get(ParameterSet::SPOT_LATITUDE);
    $longitude    = $request->get(ParameterSet::SPOT_LONGITUDE);
    $tags         = $request->get(ParameterSet::SPOT_TAGS);
    $uploadedFile = $request->files->get('image');

    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response->setError('Authentication required');
      return;
    }

    // Check location
    if(   $latitude  == 0
       || $longitude == 0)
    {
      $response->setError('Invalid location');
      return;
    }

    // Check for valid name
    if(strlen($name) == 0)
    {
      $response->setError('Invalid spot name');
      return;
    }

    // Check for valid picture
    if(   $uploadedFile == null
       || $uploadedFile->isValid() == false
       || $uploadedFile->getMimeType() != 'image/jpeg')
    {
      $response->setError('Invalid file received');
      return;
    }

    // New spot
    $spot = new Spot();
    $spot->setName($name);
    $spot->setUser($user);
    $spot->setDescription($description);
    $spot->setSecretSpot($secretSpot);

    // New picture
    $picture = new Picture();
    $picture->setUser($user);
    $picture->setLatitude($latitude);
    $picture->setLongitude($longitude);
    $picture->setPublished(true);

    $spot->addPicture($picture);

    // Persist entities
    $em = $this->getDoctrine()->getManager();
    $em->persist($spot);
    $em->persist($picture);


    // Create the directory for the new pictures
    $destinationDirectory = 'pictures/'.$picture->getCreated()->format('Y/m/d/');
    $fs = new Filesystem();
    try
    {
      $fs->mkdir($destinationDirectory);
    }
    catch (IOExceptionInterface $e)
    {
      $response->setError("An error occurred while creating your directory at ".$e->getPath());
      return;
    }

    // Flush
    try
    {
      $em->flush();
    }
    catch (DBALException $e)
    {
      $response->setError("Flush exception ".$e->getMessage());
      return;
    }

    // Move the temporarily stored file to a convenient location
    $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');

    // Generate thumbnail
    $this->thumb($movedFile->getPathname(), 180);

    $response->addData('successful', true);
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  private function getPicturesByNewest(&$response)
  {
    $minimumClientVersion = 'V0.0.1';

     $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsSpotsBundle:Picture');

    $jPictures = array();
    foreach($repository->getPicturesByNewest() as &$picture)
    {
      $jPictures[] = $picture->toJson();
    }

    $response->addData(ParameterSet::PICTURE_LIST, $jPictures);
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  private function getPicturesBySpotId($request, &$response)
  {
    $minimumClientVersion = 'V0.0.2';

    // Get parameters
    $spotId = $request->get(ParameterSet::SPOT_SPOT_ID);

    // Get spot
    $em = $this->getDoctrine()->getManager();
    $spotRepository = $em->getRepository('InstaspotsSpotsBundle:Spot');

    $spot = $spotRepository->findOneById($spotId);

    // Get pictures
    $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
    $pictures = $pictureRepository->findBySpot($spot,
                                               array('created' => 'DESC'));

    $jPictures = array();
    foreach($pictures as &$picture)
    {
      $jPictures[] = $picture->toJson();
    }

    $response->addData(ParameterSet::PICTURE_LIST, $jPictures);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getPicturesByUserId($request, &$response)
  {
    $minimumClientVersion = 'V0.0.2';

    // Get parameters
    $userId = $request->get(ParameterSet::USER_USER_ID);

    // Get user
    $em = $this->getDoctrine()->getManager();
    $userRepository = $em->getRepository('InstaspotsUserBundle:User');
    $user = $userRepository->findOneById($userId);

    if(empty($user))
    {
      $response->setError('User with id \''.$userId.'\' not found');
      return;
    }

    // Get pictures
    $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
    $pictureList = $pictureRepository->findByUser($user,
                                                  array('created' => 'DESC'));

    $jPictures = array();
    foreach($pictureList as &$picture)
    {
      $jPictures[] = $picture->toJson();
    }

    $response->addData(ParameterSet::PICTURE_LIST, $jPictures);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getSpotsByDistance($request, &$response)
  {
    $minimumClientVersion = 'V0.0.2';

    // Get parameters
    $latitude       = $request->get(ParameterSet::SPOT_LATITUDE);
    $longitude      = $request->get(ParameterSet::SPOT_LONGITUDE);
    $maxDistance_km = $request->get(ParameterSet::SPOT_DISTANCE_KM);

    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsSpotsBundle:Spot');

    $jSpots = array();
    foreach($repository->getArrayByDistance($latitude,
                                            $longitude,
                                            $maxDistance_km)
            as &$spot)
    {
      $jSpot = array();

      $jSpot[ParameterSet::SPOT_SPOT_ID    ] = $spot[0]['id'];
      $jSpot[ParameterSet::SPOT_NAME       ] = $spot[0]['name'];
      $jSpot[ParameterSet::SPOT_DESCRIPTION] = $spot[0]['description'];
      $jSpot[ParameterSet::SPOT_SECRET_SPOT] = $spot[0]['secretSpot'];
      $jSpot[ParameterSet::SPOT_LATITUDE   ] = $spot[0]['latitude'];
      $jSpot[ParameterSet::SPOT_LONGITUDE  ] = $spot[0]['longitude'];
      $jSpot[ParameterSet::SPOT_DISTANCE_KM] = (float)$spot['distance'];

      $picture1 = new Picture();
      $picture1->setId     ($spot[0]['picture1']['id']);
      $picture1->setCreated($spot[0]['picture1']['created']);

      $jSpot[ParameterSet::SPOT_PICTURE_PICTURE_ID_1] = $picture1->getId();
      $jSpot[ParameterSet::SPOT_PICTURE_URL_1       ] = $picture1->getUrl();

      $picture2 = new Picture();
      $picture2->setId     ($spot[0]['picture2']['id']);
      $picture2->setCreated($spot[0]['picture2']['created']);

      if($picture2->getId() != $picture1->getId())
      {
        $jSpot[ParameterSet::SPOT_PICTURE_PICTURE_ID_2] = $picture2->getId();
        $jSpot[ParameterSet::SPOT_PICTURE_URL_2       ] = $picture2->getUrl();
      }
      else
      {
        $jSpot[ParameterSet::SPOT_PICTURE_PICTURE_ID_2] = -1;
        $jSpot[ParameterSet::SPOT_PICTURE_URL_2       ] = '';
      }

      $jSpots[] = $jSpot;
    }

    $response->addData(ParameterSet::SPOT_LIST, $jSpots);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getSpotsByUserId($request, &$response)
  {
    $minimumClientVersion = 'V0.0.2';

    // Get parameters
    $userId = $request->get(ParameterSet::USER_USER_ID);

    // Get user
    $em = $this->getDoctrine()->getManager();
    $userRepository = $em->getRepository('InstaspotsUserBundle:User');
    $user = $userRepository->findOneById($userId);

    if(empty($user))
    {
      $response->setError('User with id \''.$userId.'\' not found');
      return;
    }

    // Get pictures
    $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
    $pictureList = $pictureRepository->findByUser($user,
                                                  array('created' => 'DESC'));

    // Get spots
    $spotList = array();
    foreach($pictureList as &$picture)
    {
      $spot = $picture->getSpot();
      if(in_array($spot, $spotList) == false)
      {
        $spotList[]=$spot;
      }
    }

    $jSpots = array();
    foreach($spotList as &$spot)
    {
      $jSpots[] = $spot->toJson();
    }

    $response->addData(ParameterSet::SPOT_LIST, $jSpots);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function spotSignalDisappeared($request, &$response)
  {
    $minimumClientVersion = 'V0.0.9';

    // Get parameters
    $spotId = $request->get(ParameterSet::SPOT_SPOT_ID);

    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response->setError('Authentication required');
      return;
    }

    // Get spot
    $em = $this->getDoctrine()->getManager();
    $spotRepository = $em->getRepository('InstaspotsSpotsBundle:Spot');
    $spot = $spotRepository->findOneById($spotId);

    $response->addData('successful', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function pictureLike($request, &$response)
  {
    $minimumClientVersion = 'V0.0.9';

    // Get parameters
    $pictureId = $request->get(ParameterSet::PICTURE_PICTURE_ID);

    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response->setError('Authentication required');
      return;
    }

    // Get picture
    $em = $this->getDoctrine()->getManager();
    $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
    $picture = $pictureRepository->findOneById($pictureId);

    // Like the picture
    $picture->addLiker($user);
    $em->flush();

    $response->addData('successful', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function pictureUnlike($request, $response)
  {
    $minimumClientVersion = 'V0.0.9';

    // Get parameters
    $pictureId = $request->get(ParameterSet::PICTURE_PICTURE_ID);

    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response->setError('Authentication required');
      return;
    }

    // Get picture
    $em = $this->getDoctrine()->getManager();
    $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
    $picture = $pictureRepository->findOneById($pictureId);

    // Like the picture
    $picture->removeLiker($user);
    $em->flush();

    $response->addData('successful', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function pictureRemove($request, &$response)
  {
    $minimumClientVersion = 'V0.0.9';

    // Get parameters
    $pictureId = $request->get(ParameterSet::PICTURE_PICTURE_ID);

    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response->setError('Authentication required');
      return;
    }

    // Get picture
    $em = $this->getDoctrine()->getManager();
    $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
    $picture = $pictureRepository->findOneById($pictureId);

    // Check if picture belong to user TODO or superuser
    if($picture->getUser() != $user)
    {
      $response->setError('User with id \''.$user->getId().'\' can not remove picture from user \''.$picture->getUser()->getId().'\'');
      return;
    }

    // Remove the picture
    $em->remove($picture);
    $em->flush();

    $response->addData('successful', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

  //loads up the source image, resizes it and saves with -thumb in the file name
  private function thumb( $srcFile,
                          $sideInPx )
  {
    $image = imagecreatefromjpeg($srcFile);
    $width = imagesx($image);
    $height = imagesy($image);

    $thumb = imagecreatetruecolor($sideInPx, $sideInPx);

    imagecopyresized($thumb,$image,0,0,0,0,$sideInPx,$sideInPx,$width,$height);

    imagejpeg($thumb, str_replace(".jpg","-thumb.jpg",$srcFile), 85);

    imagedestroy($thumb);
    imagedestroy($image);
  }
}
