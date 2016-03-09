#!/bin/bash

# Script to quickly test the webservice.
#
# - Needed packages: httpie
#

# Webservice URL
WEBSERVICE_URL='http://spots.lowerclassclothing.com/web/webservice'

# Post command
POST_COMMAND="http --session mySession -f POST $WEBSERVICE_URL"


### login
$POST_COMMAND command=login user_username=domi user_password=$1

### Test reportProblem command
#$POST_COMMAND command=reportProblem reportTitle="Report title" reportContent="Report content"


### Test uploadNewSpot command
#$POST_COMMAND command=uploadNewSpot latitude=47.151319 longitude=9.505557 name="Skatepark Vaduz"  description="" spot_secretSpot=0 image@"testPicture.jpg"


### Test getSpotsByUserId
$POST_COMMAND command=getSpotsByUserId user_userId=3
