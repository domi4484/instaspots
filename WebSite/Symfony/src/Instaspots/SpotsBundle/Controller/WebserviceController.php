<?php

namespace Instaspots\SpotsBundle\Controller;

use Instaspots\UserBundle\Entity\User;
use Instaspots\SpotsBundle\Entity\Spot;
use Instaspots\SpotsBundle\Entity\Picture;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\FileBag;
use Symfony\Component\HttpFoundation\File\UploadedFile;

use Symfony\Component\Filesystem\Filesystem;
use Symfony\Component\Filesystem\Exception\IOExceptionInterface;

use Symfony\Component\Security\Http\Event\InteractiveLoginEvent;
use Symfony\Component\Security\Core\Authentication\Token\UsernamePasswordToken;

use FOS\UserBundle\Util\UserManipulator;

class WebserviceController extends Controller
{
  public function webserviceAction(Request $request)
  {
    if ($request->isMethod('POST') == false)
    {
      return new JsonResponse(array('error' => 'Not a post request'));
    }

    $command = $request->get('command');
    $response = array ('error'      => "",
                       'command'    => $command);
                       
    if (strlen($command) == 0)
    {
      $response['error'] = 'Empty command';
      return new JsonResponse($response);
    }
    
    switch ($command)
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
                             $request->files->get('image'));
      break;
      
      case "getPictures":
	$this->getPictures($response);
      break;

      case "getNearbySpots":
	$this->getNearbySpots($response,
                              $request->get('latitude' ),
                              $request->get('longitude'));
      break;
    
      default:
      {
	$command = $response['command'];
	$response['error'] = "Unknown command: $command";
      }
    }
    
    return new JsonResponse($response);
  }
  
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------

  private function canregister( &$response,
                                 $username )
  {
    // check if username is not empty
    if (strlen($username) == 0)
    {
      $response['error'] = 'Empty username';
      return;
    }
    
    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsUserBundle:User');
    
    $listUsers = $repository->findByUsername($username);

    if(empty($listUsers) == false)
    {
      $response['canregister'] = false;
      return;
    }
    
    $response['canregister'] = true;
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function register( &$response,
                              $username, 
                              $password,
                              $email )
  { 
    $this->canregister($response,
                       $username);
                
    if($response['canregister'] == false)
    {
      $response['registered'] = false;
      return;
    }

    $manipulator = $this->container->get('fos_user.util.user_manipulator');

    $active = true;
    $superadmin = false;
    $manipulator->create($username,
                         $password,
                         $email,
                         $active,
                         $superadmin);

    $response['registered'] = true;
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function login( &$response, 
                           $username, 
                           $password )
  {
    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsUserBundle:User');

    $listUsers = $repository->findBy(array('username' => $username));

    if(empty($listUsers))
    {
      $response['authentication'] = false;
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

    $response['authentication'] = ($encoder->isPasswordValid($user->getPassword(),
                                                             $password,
                                                             $user->getSalt())) ? "true" : "false";
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function logout()
  {
  // TODO
    //$_SESSION = array();
    //session_destroy();
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function uploadPictureToSpot( &$response,
                                         $spotId,
                                         $latitude,
                                         $longitude,
                                         $photoData )
  {
    // Check for valid picture
    if(   $uploadedFile->isValid() == false
       || $uploadedFile->getMimeType() != 'image/jpeg')
    {
      $response['error'] = 'Invalid file received';
      return;
    }
  
    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response['error'] = 'Authentication required';
      return;
    }
    
    // Get spot
    $em = $this->getDoctrine()->getManager();
    $spotRepository = $em->getRepository('InstaspotsSpotsBundle:Spot');
    
    $spot = $spotRepository->findOneById($id);
  
    // New picture
    $picture = new Picture();
    $picture->setUser($user);
    $picture->setSpot($spot);
    $picture->setLatitude($latitude);
    $picture->setLongitude($longitude);
    $picture->setPublished(true);
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
      $response['error'] = "An error occurred while creating your directory at ".$e->getPath();
      return;
    }
  
    // Flush
    $em->flush();

    // Move the temporarily stored file to a convenient location
    $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');
    
    // Generate thumbnail
    $this->thumb($movedFile->getPathname(), 180);
    
    $response['successful'] = true;
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function uploadNewSpot( &$response,
                                   $latitude,
                                   $longitude,
                                   $name,
                                   $description,
                                   UploadedFile $uploadedFile )
  {
    // Check for valid picture
    if(   $uploadedFile->isValid() == false
       || $uploadedFile->getMimeType() != 'image/jpeg')
    {
      $response['error'] = 'Invalid file received';
      return;
    }
  
    // Get current user
    $user = $this->getUser();
    if($user == null)
    {
      $response['error'] = 'Authentication required';
      return;
    }
  
    // New spot
    $spot = new Spot();
    $spot->setName($name);
    $spot->setUser($user);
    $spot->setDescription($description);
  
    // New picture
    $picture = new Picture();
    $picture->setUser($user);
    $picture->setSpot($spot);
    $picture->setLatitude($latitude);
    $picture->setLongitude($longitude);
    $picture->setPublished(true);
    
    
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
      $response['error'] = "An error occurred while creating your directory at ".$e->getPath();
      return;
    }
  
    // Flush
    $em->flush();

    // Move the temporarily stored file to a convenient location
    $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');
    
    // Generate thumbnail
    $this->thumb($movedFile->getPathname(), 180);
    
    $response['successful'] = true;
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getPictures( &$response )
  {
    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsSpotsBundle:Picture');

    $jPictures = array();
    foreach($repository->getNews() as &$picture)
    {
      $jPicture = array();
      $jPicture['id']          = $picture->getId();
      $jPicture['latitude']    = $picture->getLatitude();
      $jPicture['longitude'  ] = $picture->getLongitude();
      $jPicture['url']         = $picture->getUrl();

      $jPicture['name'       ] = $picture->getSpot()->getName();
      $jPicture['description'] = $picture->getSpot()->getDescription();
      $jPicture['score'      ] = $picture->getSpot()->getScore();

      $jPicture['username'] = $picture->getUser()->getUsername();

      $jPictures[] = $jPicture;
    }

    $response['pictures'] = $jPictures;
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getNearbySpots( &$response,
                                    $latitude,
                                    $longitude )
  {
    $repository = $this->getDoctrine()
                         ->getManager()
                         ->getRepository('InstaspotsSpotsBundle:Spot');

    $result = query("SELECT id, name, description, latitude, longitude, SQRT( POW(111 * (latitude - '%f'), 2) + POW(111 * (%f - longitude) * COS(latitude / 57.3), 2)) AS distance FROM SPOTS HAVING distance < 150 ORDER BY distance",
                    $latitude,
                    $longitude);

    if ($result['error'])
    {
      $response['error'] = 'Select query failed';
      return;
    }

    $response['spots'] = $result['result'];


  }

  //-----------------------------------------------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------------------------------------------

  //loads up the source image, resizes it and saves with -thumb in the file name
  private function thumb($srcFile, $sideInPx)
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

