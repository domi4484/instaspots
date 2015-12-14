<?php

namespace Instaspots\SpotsBundle\Controller;

// Bundle imports --------------------------
use Instaspots\UserBundle\Entity\User;
use Instaspots\SpotsBundle\Entity\Spot;
use Instaspots\SpotsBundle\Entity\Picture;

use Instaspots\SpotsBundle\Controller\Response;
use Instaspots\SpotsBundle\Controller\ApplicationHelper;

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

// FOS imports -----------------------------
use FOS\UserBundle\Util\UserManipulator;

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
      case "login":
        $this->login($response,
                     $request->get('username'),
                     $request->get('password'));
      break;

      case "logout":
        $this->logout();
      break;

      case "canregister":
        $this->canregister($response,
                           $request->get('username'));
      break;

      case "register":
        $this->register($response,
                        $request->get('username'),
                        $request->get('password'),
                        $request->get('email'   ));
      break;

      case "getCurrentClientVersion":
        $this->getCurrentClientVersion($response);
      break;

      case "uploadPictureToSpot":
        $this->uploadPictureToSpot($response,
                                   $request->get('id_spot'  ),
                                   $request->get('latitude' ),
                                   $request->get('longitude'),
                                   $request->files->get('image'));
      break;

      case "uploadNewSpot":
        $this->uploadNewSpot($response,
                             $request->get('latitude'   ),
                             $request->get('longitude'  ),
                             $request->get('name'       ),
                             $request->get('description'),
                             $request->get('spot_secretSpot'),
                             $request->files->get('image'));
      break;

      case "getPicturesByNewest":
        $this->getPicturesByNewest($response);
      break;

      case "getPicturesBySpotId":
        $this->getPicturesBySpotId($response,
                                   $request->get('id_spot'));
      break;

      case "getPicturesByUserId":
        $this->getPicturesByUserId($response,
                                   $request->get('id_user'));
      break;

      case "getSpotsByDistance":
        $this->getSpotsByDistance($response,
                                  $request->get('latitude' ),
                                  $request->get('longitude'),
                                  $request->get('maxDistance_km'));
      break;

      case "getSpotsByUserId":
        $this->getSpotsByUserId($response,
                                $request->get('id_user'));
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

  private function canregister( &$response,
                                 $username )
  {
    $minimumClientVersion = 'V0.0.1';

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
      $response->addData('canregister', false);
      return;
    }

    $response->addData('canregister', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function register( &$response,
                              $username,
                              $password,
                              $email )
  {
    $minimumClientVersion = 'V0.0.1';

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
    $this->canregister($response,
                       $username);

    if($response->getData('canregister') == false)
    {
      $response->addData('registered', false);
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
    $this->login($response,
                 $username,
                 $password);

    $response->addData('registered', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  /**
   * Login
   *
   * @param \Response $response
   * @param \String   $username
   * @param \String   $password
   * @return WebserviceController
   */
  private function login( &$response,
                           $username,
                           $password )
  {
    $minimumClientVersion = 'V0.0.1';

    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsUserBundle:User');

    $listUsers = $repository->findBy(array('username' => $username));

    if(empty($listUsers))
    {
      $response->addData('authentication', false);
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

    $response->addData('id_user',        $user->getId());
    $response->addData('authentication', $validPassword);
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

  private function getCurrentClientVersion( &$response )
  {
    $response->addData('version', 'V0.0.2');
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function uploadPictureToSpot( &$response,
                                         $spotId,
                                         $latitude,
                                         $longitude,
                                         UploadedFile $uploadedFile )
  {
    $minimumClientVersion = 'V0.0.1';

    // Check for valid picture
    if(   $uploadedFile->isValid() == false
       || $uploadedFile->getMimeType() != 'image/jpeg')
    {
      $response->setError('Invalid file received');
      return;
    }

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
    $em->flush();

    // Move the temporarily stored file to a convenient location
    $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');

    // Generate thumbnail
    $this->thumb($movedFile->getPathname(), 180);

    $response->addData('successful', true);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function uploadNewSpot( &$response,
                                   $latitude,
                                   $longitude,
                                   $name,
                                   $description,
                                   $secretSpot,
                                   UploadedFile $uploadedFile )
  {
    $minimumClientVersion = 'V0.0.3';

    // Check for valid picture
    if(   $uploadedFile->isValid() == false
       || $uploadedFile->getMimeType() != 'image/jpeg')
    {
      $response->setError('Invalid file received');
      return;
    }

    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response->setError('Authentication required');
      return;
    }

    // New spot
    $spot = new Spot();
    $spot->setName($name);
    $spot->setUser($user);
    $spot->setDescription($description);

    if(ApplicationHelper::compareVersions($response->getClientVersion(), 'V0.0.3') >= 0)
    {
      $spot->setSecretSpot($secretSpot);
    }

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
    $em->flush();

    // Move the temporarily stored file to a convenient location
    $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');

    // Generate thumbnail
    $this->thumb($movedFile->getPathname(), 180);

    $response->addData('successful', true);
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  private function getPicturesByNewest( &$response )
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

    $response->addData('pictures', $jPictures);
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  private function getPicturesBySpotId( &$response,
                                         $spotId)
  {
    $minimumClientVersion = 'V0.0.2';

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

    $response->addData('pictures', $jPictures);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getPicturesByUserId( &$response,
                                          $userId )
  {
    $minimumClientVersion = 'V0.0.2';

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

    $response->addData('pictures', $jPictures);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getSpotsByDistance( &$response,
                                        $latitude,
                                        $longitude,
                                        $maxDistance_km )
  {
    $minimumClientVersion = 'V0.0.2';

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

      $jSpot['id']          = $spot[0]['id'];
      $jSpot['name']        = $spot[0]['name'];
      $jSpot['description'] = $spot[0]['description'];
      $jSpot['latitude']    = $spot[0]['latitude'];
      $jSpot['longitude']   = $spot[0]['longitude'];
      $jSpot['secretSpot']  = $spot[0]['secretSpot'];
      $jSpot['distance_km'] = (float)$spot['distance'];

      $picture1 = new Picture();
      $picture1->setId     ($spot[0]['picture1']['id']);
      $picture1->setCreated($spot[0]['picture1']['created']);

      $jSpot['pictureId1']  = $picture1->getId();
      $jSpot['pictureUrl1'] = $picture1->getUrl();

      $picture2 = new Picture();
      $picture2->setId     ($spot[0]['picture2']['id']);
      $picture2->setCreated($spot[0]['picture2']['created']);

      if($picture2->getId() != $picture1->getId())
      {
        $jSpot['pictureId2']  = $picture2->getId();
        $jSpot['pictureUrl2'] = $picture2->getUrl();
      }
      else
      {
        $jSpot['pictureId2']  = -1;
        $jSpot['pictureUrl2'] = '';
      }

      $jSpots[] = $jSpot;
    }

    $response->addData('spots', $jSpots);
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getSpotsByUserId( &$response,
                                      $userId )
  {
    $minimumClientVersion = 'V0.0.2';

    // Get user
    $em = $this->getDoctrine()->getManager();
    $userRepository = $em->getRepository('InstaspotsUserBundle:User');
    $user = $userRepository->findOneById($userId);

    if(empty($user))
    {
      $response->setError('User with id \''.$userId.'\' not found');
      return;
    }

    // Get spots
    $spotRepository = $em->getRepository('InstaspotsSpotsBundle:Spot');
    $spotList = $spotRepository->findByUser($user,
                                               array('modified' => 'DESC'));

    $jSpots = array();
    foreach($spotList as &$spot)
    {
      $jSpots[] = $spot->toJson();
    }

    $response->addData('spots', $jSpots);
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
