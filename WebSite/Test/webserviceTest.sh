#!/bin/bash

# Script to quickly test the webservice.
#
# - Needed packages: httpie
#

# Webservice URL
WEBSERVICE_URL='http://spots.lowerclassclothing.com/web/webservice'

# Post command
POST_COMMAND="http -f POST $WEBSERVICE_URL"


### Test reportProblem command
$POST_COMMAND command=reportProblem reportTitle="Report title" reportContent="Report content"
