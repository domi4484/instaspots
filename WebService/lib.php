<?php

//setup db connection
//$link = mysqli_connect("localhost","sk8mapp");   // Altervista
//mysqli_select_db($link, "my_sk8mapp");

$link = mysqli_connect("lowerc3.mysql.db.internal", "lowerc3_spots", "gaaemnWj");     // Hostpoint
mysqli_select_db($link, "lowerc3_spots");

//executes a given sql query with the params and returns an array as result
function query() {
	global $link;
	$debug = false;
	
	//get the sql query
	$args = func_get_args();
	$sql = array_shift($args);

	//secure the input
	for ($i=0;$i<count($args);$i++) {
		$args[$i] = urldecode($args[$i]);
		$args[$i] = mysqli_real_escape_string($link, $args[$i]);
	}
	
	//build the final query
	$sql = vsprintf($sql, $args);
	
	if ($debug) print $sql;
	
	//execute and fetch the results
	$result = mysqli_query($link, $sql);
	if (mysqli_errno($link)==0 && $result) {
		
		$rows = array();

		if ($result!==true)
		while ($d = mysqli_fetch_assoc($result)) {
			array_push($rows,$d);
		}
		
		//return json
		return array('result'=>$rows);
		
	}
	else 
	{
		//error
		return array('error'=>'Database error: '.mysqli_error($link));
	}
}

//-----------------------------------------------------------------------------------------------------------------------------

//loads up the source image, resizes it and saves with -thumb in the file name
function thumb($srcFile, $sideInPx) {

  $image = imagecreatefromjpeg($srcFile);
  $width = imagesx($image);
  $height = imagesy($image);
  
  $thumb = imagecreatetruecolor($sideInPx, $sideInPx);
  
  imagecopyresized($thumb,$image,0,0,0,0,$sideInPx,$sideInPx,$width,$height);
  
  imagejpeg($thumb, str_replace(".jpg","-thumb.jpg",$srcFile), 85);
  
  imagedestroy($thumb);
  imagedestroy($image);
}

//-----------------------------------------------------------------------------------------------------------------------------

function checkPhotoData($photoData)
{
  // Undefined | Multiple Files | $_FILES Corruption Attack
  // If this request falls under any of them, treat it invalid.
  if (!isset($photoData['error']))
    throw new RuntimeException('Invalid parameters 1.');

  if (is_array($photoData['error']))
    throw new RuntimeException('Invalid parameters 2.');
      
  // Check $_FILES['upfile']['error'] value.
  switch ($photoData['error']) 
  {
    case UPLOAD_ERR_OK:
      break;
    case UPLOAD_ERR_NO_FILE:
      throw new RuntimeException('No file sent.');
    case UPLOAD_ERR_INI_SIZE:
    case UPLOAD_ERR_FORM_SIZE:
      throw new RuntimeException('Exceeded filesize limit.');
    default:
      throw new RuntimeException('Unknown errors.');
  }

  // You should also check filesize here. 
  if ($photoData['size'] > 10000000) 
    throw new RuntimeException('Exceeded filesize limit.');

  // DO NOT TRUST $_FILES['upfile']['mime'] VALUE !!
  // Check MIME Type by yourself.
  $finfo = new finfo(FILEINFO_MIME_TYPE);
  if (false === $ext = array_search($finfo->file($photoData['tmp_name']),
                                    array('jpg' => 'image/jpeg',
                                          'png' => 'image/png',
                                          'gif' => 'image/gif'),
                                    true))
  {
    throw new RuntimeException('Invalid file format.');
  }
}

?>
