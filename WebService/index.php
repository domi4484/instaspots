<?php
/********************************************************************
 *                                                                 *
 * InstaSpots                                                      *
 *                                                                 *
 * Author:       Damiano Lombardi                                  *
 * Created:      21.05.2014                                        *
 *                                                                 *
 * Copiright (c) 2014 Damiano Lombardi                             *
 *                                                                 *
********************************************************************/

session_start();

require("lib.php");
require("api.php");

header("Content-Type: application/json");

$response = array ('error'      => "",
                   'command'    => $_POST['command']);
    
switch ($response['command'])
{
  case "":
  {
    $postContent = implode("Elemento:", $_POST);
    $response['error'] = "Empty command. Post: $postContent";
  }
  break;
  case "login": 
    login($response,
          $_POST['username'], 
          $_POST['password']); 
  break;
  
  case "logout":
    logout();
  break;

  case "canregister":
    canregister($response,
                $_POST['username']); 
  break;

  case "register":
    register($response,
             $_POST['username'],
             $_POST['password'],
             $_POST['email'   ]);
  break;
  
  case "uploadPictureToSpot":
    uploadPictureToSpot($response,
                        $_SESSION['id'       ],
                        $_POST   ['id_spot'  ],
                        $_POST   ['latitude' ],
                        $_POST   ['longitude'],
                        $_FILES  ['image'    ]);
  break;
  
  case "uploadNewSpot":
    uploadNewSpot($response,
                  $_SESSION['id'         ],
                  $_POST   ['latitude'   ],
                  $_POST   ['longitude'  ],
                  $_POST   ['name'       ],
                  $_POST   ['description'],
                  $_FILES  ['image'      ]);
  break;
  
  case "getPictures":
    getPictures($response);
  break;

  case "getNearbySpots":
    getNearbySpots($response,
                   $_POST   ['latitude'   ],
                   $_POST   ['longitude'  ]);
  break;
 
  default:
  {
    $command = $response['command'];
    $response['error'] = "Unknown command: $command";
  }
}
  
print json_encode($response);
exit();
?>
