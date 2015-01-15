<?php

function canregister( &$response,
                       $username )
{
  // check if username is not empty
  if (strlen($username) == 0)
  {
    $response['error'] = 'Empty username';
    return;
  }

  // check if username exists
  $login = query("SELECT username FROM USERS WHERE username='%s' limit 1",
                 $username);

  if (count($login['result']) > 0) 
  {
    $response['canregister'] = false;
    return;
  }

  $response['canregister'] = true;
}

//-----------------------------------------------------------------------------------------------------------------------------

function register( &$response,
                    $username, 
                    $password,
                    $email )
{
  // check if username exists
  // canregister($username);

  //try to register the user
  $result = query("INSERT INTO USERS(username, password, email) VALUES('%s','%s','%s')", 
                  $username,
                  $password,
                  $email);

  if ($result['error'])
  { 
    $response['error'] = 'Registration failed';
    return;
  }

  $response['registered'] = true;
}

//-----------------------------------------------------------------------------------------------------------------------------

function login( &$response, 
                 $username, 
                 $password )
{
  $result = query("SELECT id, username FROM USERS WHERE username='%s' AND password='%s' limit 1",
                  $username,
                  $password);
 
  if (count($result['result']) <= 0)
  {
    //not authorized
    $response['authentication'] = false;
    return;
  } 

  //authorized
  $_SESSION['id'] = $result['result'][0]['id'];
  $response['authentication'] = true;
}

//-----------------------------------------------------------------------------------------------------------------------------

function logout()
{
  $_SESSION = array();
  session_destroy();
}

//-----------------------------------------------------------------------------------------------------------------------------

function uploadPictureToSpot( &$response,
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

function uploadNewSpot( &$response,
                         $id,
                         $latitude,
                         $longitude,
                         $name,
                         $description,
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
  
  // Insert spot in db
  $result = query("INSERT INTO SPOTS(id_user, latitude, longitude, name, description) VALUES('%d', '%f', '%f', '%s', '%s')", 
                  $id,
                  $latitude,
                  $longitude,
                  $name,
                  $description);
  if ($result['error'])
  {
    // rollback transaction
    mysqli_rollback($link);
  
    $response['error'] = 'Upload database problem. INSERT INTO SPOTS: '.$result['error'];
    return;
  } 
    
    
  //get the last automatically generated ID
  $spotId = mysqli_insert_id($link);
  
  
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

function getPictures( &$response )
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

function getNearbySpots( &$response,
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

//-----------------------------------------------------------------------------------------------------------------------------

?>




