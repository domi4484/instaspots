#!/bin/bash

# Script to quickly test the webservice.
#
# - Needed packages: httpie
#

# Webservice URL
WEBSERVICE_URL='https://lowerspot.com/web/webservice'

# Post command
POST_COMMAND="http --session mySession -f POST $WEBSERVICE_URL"


### login
$POST_COMMAND command=login user_username=$1 user_password=$2

### Test reportProblem command
#$POST_COMMAND command=reportProblem reportTitle="Report title" reportContent="Report content"


### Test uploadNewSpot command
#$POST_COMMAND command=uploadNewSpot latitude=47.151319 longitude=9.505557 name="Skatepark Vaduz"  description="" spot_secretSpot=0 image@"testPicture.jpg"

### Test getSpotById
#$POST_COMMAND command=getSpotById spot_spotId=$1

### Test getSpotsByUserId
#$POST_COMMAND command=getSpotsByUserId user_userId=3

### Test pictureRemove
$POST_COMMAND command=pictureRemove picture_pictureId=$3

