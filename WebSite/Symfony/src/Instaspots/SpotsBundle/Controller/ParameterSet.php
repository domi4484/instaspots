<?php

namespace Instaspots\SpotsBundle\Controller;

class ParameterSet
{
  const USER_USER_ID       = "id_user";
  const USER_USERNAME      = "username";
  const USER_PASSWORD      = "password";
  const USER_EMAIL         = "email";
  const USER_AUTHENTICATED = "authentication";
  const USER_REGISTERED    = "registered";

  const REPORT_TITLE   = "reportTitle";
  const REPORT_CONTENT = "reportContent";

  const APPLICATION_VERSION = "version";
  const PARAM_SUCCESSFUL    = "successful";

  const SPOT_LIST          = "spots";
  const SPOT_SPOT_ID       = "id_spot";
  const SPOT_NAME          = "name";
  const SPOT_DESCRIPTION   = "description";
  const SPOT_SECRET_SPOT   = "spot_secretSpot";
  const SPOT_LATITUDE      = "latitude";
  const SPOT_LONGITUDE     = "longitude";
  const SPOT_DISTANCE_KM   = "distance_km";
  const SPOT_PICTURE_URL_1 = "pictureUrl1";
  const SPOT_PICTURE_URL_2 = "pictureUrl2";

  const PICTURE_LIST             = "pictures";
  const PICTURE_PICTURE_ID       = "id";
  const PICTURE_USER_ID          = "id_user";
  const PICTURE_USER_USERNAME    = "username";
  const PICTURE_SPOT_ID          = "id_spot";
  const PICTURE_SPOT_NAME        = "name";
  const PICTURE_SPOT_DESCRIPTION = "description";
  const PICTURE_URL              = "url";
  const PICTURE_CREATED          = "created";
}
