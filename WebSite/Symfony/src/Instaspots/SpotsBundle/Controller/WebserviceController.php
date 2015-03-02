<?php

namespace Instaspots\SpotsBundle\Controller;

use Instaspots\UserBundle\Entity\User;
use Instaspots\SpotsBundle\Entity\Spot;
use Instaspots\SpotsBundle\Entity\Picture;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\JsonResponse;

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
		      $_SESSION['id'         ],
		      $_POST   ['latitude'   ],
		      $_POST   ['longitude'  ],
		      $_POST   ['name'       ],
		      $_POST   ['description'],
		      $_FILES  ['image'      ]);
      break;
      
      case "getPictures":
	$this->getPictures($response);
      break;

      case "getNearbySpots":
	$this->getNearbySpots($response,
		      $_POST   ['latitude'   ],
		      $_POST   ['longitude'  ]);
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

//    // Création de l'entité
//    $user = new User();
//    $user->setUsername($username);
//    $user->setPassword($password);
//    $user->setEmail($email);
//
//     // On récupère l'EntityManager
//     $em = $this->getDoctrine()->getManager();
//
//     // Étape 1 : On « persiste » l'entité
//     $em->persist($user);
//
//     // Étape 2 : On « flush » tout ce qui a été persisté avant
//     $em->flush();

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
                                   $id,
                                   $latitude,
                                   $longitude,
                                   $name,
                                   $description,
                                   $photoData )
  {
    $em = $this->getDoctrine()->getManager();

    $user = $em
      ->getRepository('InstaspotsSpotsBundle:User')
      ->find($id)
    ;

    if (null === $user) {
      $response['error'] = 'Authorization required';
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
    $result = query("SELECT id, id_spot, id_user, created FROM PICTURES ORDER BY created DESC");

    if ($result['error'])
    { 
      $response['error'] = 'Select query failed';
      return;
    }
  
    $pictures = $result['result'];
    foreach($pictures as &$picture)
    {
      $query_spot = query("SELECT name, description, score, latitude, longitude, diameter_m FROM SPOTS WHERE id='%d'",
                          $picture['id_spot']);
      $spot = $query_spot['result'][0];
      $picture['name'       ] = $spot['name'       ];
      $picture['description'] = $spot['description'];
      $picture['score'      ] = $spot['score'      ];
      $picture['latitude'   ] = $spot['latitude'   ];
      $picture['longitude'  ] = $spot['longitude'  ];
      $picture['diameter_m' ] = $spot['diameter_m' ];
    
      $query_user = query("SELECT username FROM USERS WHERE id='%d'",
                          $picture['id_user']);
      $picture['username'] = $query_user['result'][0]['username'];
    }

    $response['pictures'] = $pictures;
  }

//-----------------------------------------------------------------------------------------------------------------------------

  private function getNearbySpots( &$response,
                          $latitude,
                          $longitude)
  {
    $result = query("SELECT id, name, description, picture_url1, picture_url2, latitude, longitude, SQRT( POW(111 * (latitude - '%f'), 2) + POW(111 * (%f - longitude) * COS(latitude / 57.3), 2)) AS distance FROM SPOTS HAVING distance < 150 ORDER BY distance",
                    $latitude,
                    $longitude);

    if ($result['error'])
    {
      $response['error'] = 'Select query failed';
      return;
    }

    $response['spots'] = $result['result'];

    //picture_url_1
    //picture_url_2
  } 
}

