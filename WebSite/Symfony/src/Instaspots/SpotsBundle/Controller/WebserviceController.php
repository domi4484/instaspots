<?php

namespace Instaspots\SpotsBundle\Controller;

use Instaspots\UserBundle\Entity\User;
use Instaspots\SpotsBundle\Entity\Spot;
use Instaspots\SpotsBundle\Entity\Picture;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\JsonResponse;

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

    $command = $request->request->get('command');
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
                     $request->request->get('username'), 
                     $request->request->get('password')); 
      break;
      
      case "logout":
        $this->logout();
      break;

      case "canregister":
	    $this->canregister($response,
                               $request->request->get('username'));
      break;

      case "register":
	$this->register($response,
                        $request->request->get('username'),
                        $request->request->get('password'),
                        $request->request->get('email'   ));
      break;
      
      case "uploadPictureToSpot":
	$this->uploadPictureToSpot($response,
			           $_SESSION['id'       ],
			           $_POST   ['id_spot'  ],
			           $_POST   ['latitude' ],
		                   $_POST   ['longitude'],
			           $_FILES  ['image'    ]);
      break;
      
      case "uploadNewSpot":
	$this->uploadNewSpot($response,
                         $request->request->get('latitude'   ),
		                 $request->request->get('longitude'  ),
		                 $request->request->get('name'       ),
	       	             $request->request->get('description'),
		                 $_FILES  ['image'      ]);
      break;
      
      case "getPictures":
	$this->getPictures($response);
      break;

      case "getNearbySpots":
	$this->getNearbySpots($response,
                              $request->request->get('latitude' ),
                              $request->request->get('longitude'));
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
                                     $id,
                                     $spotId,
                                     $latitude,
                                     $longitude,
                                     $photoData )
  {
    //check if a user ID is passed
    if (!$id)
    {
      $response['error'] = 'Authorization required';
      return;
    }
    
    //database link
    global $link;
    
    // Open the transaction
    $result = query("START TRANSACTION");
    if ($result['error'])
    {
      $response['error'] = 'Error starting transaction';
      return;
    }
    
    // Spot in database?
    $spot = query("SELECT id FROM SPOTS WHERE spot='%d' limit 1",
		  $spotId);
    if (count($spot['result']) == 0) 
    {
      $response['error'] = "Spot id not found: $spotId";
      return;
    }
    
    // Insert photo in db
    $result = query("INSERT INTO PICTURES(id_user, id_spot, latitude, longitude) VALUES('%d', '%d', '%f', '%f')", 
		    $id,
		    $spotId,
		    $latitude,
		    $longitude);
    if ($result['error'])
    {
      // rollback transaction
      mysqli_rollback($link);
    
      $response['error'] = 'Upload database problem.'.$result['error'];
      return;
    }

    //get the last automatically generated ID
    $IdPhoto = mysqli_insert_id($link);
      
    try 
    {
      checkPhotoData($photoData);
    }
    catch (RuntimeException $e) 
    {
      // rollback transaction
      mysqli_rollback($link);
    
      $messaggio = $e->getMessage();
      $response['error'] = "Eccezione cacciata: $messaggio";
      return;
    }
  
    //move the temporarily stored file to a convenient location
    if (!move_uploaded_file($photoData['tmp_name'], "upload/".$IdPhoto.".jpg"))
    {
      // rollback transaction
      mysqli_rollback($link);
      
      $response['error'] = 'Upload on server problem';
      return;
    }
    
    //file moved, all good, generate thumbnail
    thumb("upload/".$IdPhoto.".jpg", 180);
    
    // Commit transaction
    mysqli_commit($link);
    
    $response['successful'] = true;
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function uploadNewSpot( &$response,
                                   $latitude,
                                   $longitude,
                                   $name,
                                   $description,
                                   $photoData )
  {
      if (!$this->get('security.authorization_checker')->isGranted('IS_AUTHENTICATED_FULLY'))
      {
         $response['error'] = 'Authentication required';
          return;
      }

      $user = $this->getUser();


      $response['useratore'] = $user->getId();
      return;

    if (null === $user) {
      $response['error'] = 'Authentication required';
      return;
    }
  
    // New spot
    $spot = new Spot($name);
    $spot->setUser($user);
    $spot->setDescription($description);
  
    // New picture
    $picture = new Picture();
    $picture->setUser($user);
    $picture->setSpot($spot);
    $picture->setLatitude($latitude);
    $picture->setLongitude($longitude);
    $picture->setPublished(true);
  
  
    try 
    {
      checkPhotoData($photoData);
    }
    catch (RuntimeException $e) 
    {
      $messaggio = $e->getMessage();
      $response['error'] = "Eccezione cacciata: $messaggio";
      return;
    }
    
    $em->persist($spot);
    $em->persist($picture);
    $em->flush();
  
    //move the temporarily stored file to a convenient location
    if (!move_uploaded_file($photoData['tmp_name'], "upload/".$picture->getId().".jpg"))
    {
      $em->remove($spot);
      $em->remove($picture);
      $em->flush();
      $response['error'] = 'Upload on server problem';
      return;
    }
    
    //file moved, all good, generate thumbnail
    thumb("upload/".$picture->getId().".jpg", 180);
    
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
}

