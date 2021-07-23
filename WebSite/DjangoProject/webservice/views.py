from spots.models import Spot
from spots.models import Picture
from django.contrib.auth.models import User

from webservice.permissions import IsOwnerOrReadOnly
from webservice.serializers import SpotSerializer
from webservice.serializers import PictureSerializer
from webservice.serializers import UserSerializer

from rest_framework import generics
from rest_framework import status
from rest_framework import permissions
from rest_framework.views import APIView
from rest_framework.response import Response

from django.contrib.auth import authenticate, login, logout
from django.contrib.gis.geos import GEOSGeometry
from django.contrib.gis.measure import D  # D for distance
from django.contrib.gis.db.models.functions import Distance
from django.core.exceptions import ValidationError
from django.core.validators import validate_email
from django.http import Http404
from django.db import transaction

# import the logging library
import logging

# Get an instance of a logger
logger = logging.getLogger(__name__)


class UserList(generics.ListAPIView):

    def perform_create(self, serializer):
        serializer.save(user=self.request.user)

    queryset = User.objects.all()
    serializer_class = UserSerializer


class UserDetail(generics.RetrieveAPIView):
    queryset = User.objects.all()
    serializer_class = UserSerializer


class UserDetailSpotList(generics.ListAPIView):
    serializer_class = SpotSerializer

    def get_queryset(self):
        if self.request.method == 'GET':
            userId = self.kwargs['pk']
            return Spot.objects.filter(user=userId)


class UserDetailPicturesList(generics.ListAPIView):
    serializer_class = PictureSerializer

    def get_queryset(self):
        if self.request.method == 'GET':
            userId = self.kwargs['pk']
            return Picture.objects.filter(user=userId)


class SpotList(generics.ListCreateAPIView):
    """
    List all spots, or create a new spot.
    """
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)
    queryset = Spot.objects.all()
    serializer_class = SpotSerializer


class SpotDetail(generics.RetrieveUpdateDestroyAPIView):
    """
    Retrieve, update or delete a spot instance.
    """
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,
                                      IsOwnerOrReadOnly)
    queryset = Spot.objects.all()
    serializer_class = SpotSerializer


class SpotListByDistance(generics.ListCreateAPIView):
    """
    List all spots by distance.
    """
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,
                                      IsOwnerOrReadOnly)

    serializer_class = SpotSerializer

    def get_queryset(self):
        if self.request.method == 'GET':
            currentPosition = self.request.GET.get('position', None)
            if currentPosition is None:
                raise Http404("Missing parameter 'position'")
            currentPosition = GEOSGeometry(currentPosition)

            distanceKm = self.request.GET.get('distanceKm', None)
            if currentPosition is None:
                raise Http404("Missing parameter 'distanceKm'")

            queryset = Spot.objects.filter(position__distance_lte=(currentPosition, D(km=distanceKm))
                                           ).annotate(
                distance=Distance('position', currentPosition)
            ).order_by('distance')

            return queryset


class PictureList(generics.ListCreateAPIView):
    """
    List all pictures, or create a new pictures.
    """
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)
    queryset = Picture.objects.all()
    serializer_class = PictureSerializer


class PictureDetail(generics.RetrieveUpdateDestroyAPIView):
    """
    Retrieve, update or delete a picture instance.
    """
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,
                                      IsOwnerOrReadOnly)
    queryset = Picture.objects.all()
    serializer_class = PictureSerializer


class PictureListByNewest(generics.ListCreateAPIView):
    """
    List all pictures, or create a new pictures.
    """
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,
                                      IsOwnerOrReadOnly)
    queryset = Picture.objects.order_by('-created')[:20]
    serializer_class = PictureSerializer


class Webservice(APIView):
    """
    Webservice like before
    """

    _command       = ''
    _clientVersion = ''
    _error         = ''
    _warning       = ''
    _data          = {}

    def _response(self):
        """
        Prepare the response
        """
        response = { 'command' : self._command,
                     'version' : self._clientVersion,
                     'error'   : self._error,
                     'warning' : self._warning}

        if self._data:
            response.update(self._data)
        return response


    def _login(self, request):
        """
        Login
        """
        minimumClientVersion = 'V0.0.1'

        username = request.POST.get('username', '')
        password = request.POST.get('password', '')

        user = authenticate(username=username, password=password)
        if user is None:
            self._data['authentication'] = False
            return

        login(request, user)
        self._data['authentication'] = True


    def _logout(self, request):
        """
        Logout
        """
        minimumClientVersion = 'V0.0.1'

        logout(request)


    def _register(self, request):
        """
        Register a new user
        """
        minimumClientVersion = 'V0.0.1'

        if request.user.is_authenticated():
            logout(request)

        # get request data
        username = request.POST.get('username', '')
        password = request.POST.get('password', '')
        email    = request.POST.get('email',    '')

        # check if username is not empty
        if not username:
            self._error = 'Empty username'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)

        # Check if username is available
        usernameAlreadyTaken = True
        try:
            User.objects.get(username=username)
        except User.DoesNotExist:
            usernameAlreadyTaken = False

        if usernameAlreadyTaken:
            self._error = 'Username already taken'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)


        # check password lenght
        if len(password) < 6:
            self._error = 'Password must be at least 6 characters'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)

        if len(password) > 30:
            self._error = 'Password can\'t be longer than 30 characters'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)


        # validate email
        if not email:
            self._error = 'Empty email'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)

        try:
            validate_email(email)
        except ValidationError as e:
            self._error = 'Invalid email'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)

        # Check if email is available
        emailAlreadyTaken = True
        try:
            User.objects.get(email=email)
        except User.DoesNotExist:
            emailAlreadyTaken = False

        if emailAlreadyTaken:
            self._error = 'Email already in use'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)


        # Create new user
        user = User(username=username,
                    email=email)
        user.set_password(password)
        user.save()

        # Login the new user
        self._login(request)


    def _getCurrentClientVersion(self, request):
        """
        Get the current client application version
        """
        self._data['version'] = 'V0.0.4'


    def _uploadPictureToSpot(self, request):
        """
        Upload a new picture
        """

        # get request data
        spot_id   = request.POST.get('id_spot',   '')
        latitude  = request.POST.get('latitude',  '' )
        longitude = request.POST.get('longitude', '')

#        spot_id = '2'
#        latitude  = '0.0'
#        longitude = '0.0'

        # get current user
        user = request.user
        if not user.is_authenticated():
            self._error = 'Authentication required'
            return Response(self._response(), status=status.HTTP_403_FORBIDDEN)

        # get spot
        if not spot_id:
            self._error = 'Spot id is missing'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)
        try:
            spot = Spot.objects.get(pk=spot_id)
        except Spot.DoesNotExist:
            self._error = 'Spot whith id \'' + spot_id + '\' not found'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)


        # new picture
        picture = Picture()
        picture.user      = user
        picture.spot      = spot
        picture.latitude  = latitude
        picture.longitude = longitude
        picture.published = True

        with transaction.commit_manually():
            try:
                picture.save()
            except Exception as ecxeption:
                transaction.rollback()
                self._error = ecxeption.__repr__()
                return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)

            try:
                picture.saveUploadedPicture(request.files['image'])
            except Exception as ecxeption:
                transaction.rollback()
                self._error = ecxeption.__repr__()
                return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)



            self._data['picture_id'] = picture.id
            transaction.commit()


    #
    #
    #     // Check for valid picture
    #     if(   $uploadedFile->isValid() == false
    #        || $uploadedFile->getMimeType() != 'image/jpeg')
    #     {
    #       $response->setError('Invalid file received');
    #       return;
    #     }
    #
    #     // New picture
    #     $picture = new Picture();
    #     $picture->setUser($user);
    #     $picture->setLatitude($latitude);
    #     $picture->setLongitude($longitude);
    #     $picture->setPublished(true);
    #
    #     $spot->addPicture($picture);
    #
    #     $em->persist($picture);
    #
    #     // Create the directory for the new pictures
    #     $destinationDirectory = 'pictures/'.$picture->getCreated()->format('Y/m/d/');
    #     $fs = new Filesystem();
    #     try
    #     {
    #       $fs->mkdir($destinationDirectory);
    #     }
    #     catch (IOExceptionInterface $e)
    #     {
    #       $response->setError("An error occurred while creating directory at ".$e->getPath());
    #       return;
    #     }
    #
    #     // Flush
    #     $em->flush();
    #
    #     // Move the temporarily stored file to a convenient location
    #     $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');
    #
    #     // Generate thumbnail
    #     $this->thumb($movedFile->getPathname(), 180);
    #
    #     $response->addData('successful', true);
    #   }

    def _getPicturesByNewest(self, request):
        
        pass
    

    # List of commands
    _commandList = { 'login'                   : _login,
                     'logout'                  : _logout,
                     'register'                : _register,
                     'getCurrentClientVersion' : _getCurrentClientVersion,
                     'uploadPictureToSpot'     : _uploadPictureToSpot,
                     'getPicturesByNewest'     : _getPicturesByNewest }


    def post(self, request, format=None):

        self._command       = ''
        self._clientVersion = ''
        self._error         = ''
        self._warning       = ''
        self._data          = {}

        self._command       = request.POST.get('command', '')
        self._clientVersion = request.POST.get('version', '')

        # self._command = 'uploadPictureToSpot'

        # Check if command is set
        if self._command == '':
            self._error = 'Empty command'
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)

        # Check if client version is set
        if self._clientVersion == '':
            self._clientVersion = 'V0.0.0'
            self._warning       = 'Empty version'

        # Check for existing command
        if self._command not in self._commandList:
            self._error = 'Unknown command: ' + self._command
            return Response(self._response(), status=status.HTTP_400_BAD_REQUEST)

        # Execute the command
        self._commandList[self._command](self, request)

        # Send response
        return Response(self._response(), status=status.HTTP_200_OK)



    #   case "uploadNewSpot":
    #     $this->uploadNewSpot($response,
    #                          $request->get('latitude'   ),
    #                          $request->get('longitude'  ),
    #                          $request->get('name'       ),
    #                          $request->get('description'),
    #                          $request->get('spot_secretSpot'),
    #                          $request->files->get('image'));
    #   break;
    #
    #   case "getPicturesByNewest":
    #     $this->getPicturesByNewest($response);
    #   break;
    #
    #   case "getPicturesBySpotId":
    #     $this->getPicturesBySpotId($response,
    #                                $request->get('id_spot'));
    #   break;
    #
    #   case "getPicturesByUserId":
    #     $this->getPicturesByUserId($response,
    #                                $request->get('id_user'));
    #   break;
    #
    #   case "getSpotsByDistance":
    #     $this->getSpotsByDistance($response,
    #                               $request->get('latitude' ),
    #                               $request->get('longitude'),
    #                               $request->get('maxDistance_km'));
    #   break;
    #
    #   case "getSpotsByUserId":
    #     $this->getSpotsByUserId($response,
    #                             $request->get('id_user'));
    #   break;
    #




# //-----------------------------------------------------------------------------------------------------------------------------
# //-----------------------------------------------------------------------------------------------------------------------------
# //-----------------------------------------------------------------------------------------------------------------------------
# //-----------------------------------------------------------------------------------------------------------------------------
# //-----------------------------------------------------------------------------------------------------------------------------
#
#   private function uploadNewSpot( &$response,
#                                    $latitude,
#                                    $longitude,
#                                    $name,
#                                    $description,
#                                    $secretSpot,
#                                    UploadedFile $uploadedFile )
#   {
#     $minimumClientVersion = 'V0.0.3';
#
#     // Check for valid picture
#     if(   $uploadedFile->isValid() == false
#        || $uploadedFile->getMimeType() != 'image/jpeg')
#     {
#       $response->setError('Invalid file received');
#       return;
#     }
#
#     // Get current user
#     $user = $this->getUser();
#     if($user == null)
#     {
#       $response->setError('Authentication required');
#       return;
#     }
#
#     // New spot
#     $spot = new Spot();
#     $spot->setName($name);
#     $spot->setUser($user);
#     $spot->setDescription($description);
#
#     if(ApplicationHelper::compareVersions($response->getClientVersion(), 'V0.0.3') >= 0)
#     {
#       $spot->setSecretSpot($secretSpot);
#     }
#
#     // New picture
#     $picture = new Picture();
#     $picture->setUser($user);
#     $picture->setLatitude($latitude);
#     $picture->setLongitude($longitude);
#     $picture->setPublished(true);
#
#     $spot->addPicture($picture);
#
#     // Persist entities
#     $em = $this->getDoctrine()->getManager();
#     $em->persist($spot);
#     $em->persist($picture);
#
#
#     // Create the directory for the new pictures
#     $destinationDirectory = 'pictures/'.$picture->getCreated()->format('Y/m/d/');
#     $fs = new Filesystem();
#     try
#     {
#       $fs->mkdir($destinationDirectory);
#     }
#     catch (IOExceptionInterface $e)
#     {
#       $response->setError("An error occurred while creating your directory at ".$e->getPath());
#       return;
#     }
#
#     // Flush
#     $em->flush();
#
#     // Move the temporarily stored file to a convenient location
#     $movedFile = $uploadedFile->move($destinationDirectory, $picture->getId().'.jpg');
#
#     // Generate thumbnail
#     $this->thumb($movedFile->getPathname(), 180);
#
#     $response->addData('successful', true);
#   }
#
#   //-----------------------------------------------------------------------------------------------------------------------------
#
#   private function getPicturesByNewest( &$response )
#   {
#     $minimumClientVersion = 'V0.0.1';
#
#      $repository = $this->getDoctrine()
#                          ->getManager()
#                          ->getRepository('InstaspotsSpotsBundle:Picture');
#
#     $jPictures = array();
#     foreach($repository->getPicturesByNewest() as &$picture)
#     {
#       $jPictures[] = $picture->toJson();
#     }
#
#     $response->addData('pictures', $jPictures);
#   }
#
#   //-----------------------------------------------------------------------------------------------------------------------------
#
#   private function getPicturesBySpotId( &$response,
#                                          $spotId)
#   {
#     $minimumClientVersion = 'V0.0.2';
#
#     // Get spot
#     $em = $this->getDoctrine()->getManager();
#     $spotRepository = $em->getRepository('InstaspotsSpotsBundle:Spot');
#
#     $spot = $spotRepository->findOneById($spotId);
#
#     // Get pictures
#     $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
#     $pictures = $pictureRepository->findBySpot($spot,
#                                                array('created' => 'DESC'));
#
#     $jPictures = array();
#     foreach($pictures as &$picture)
#     {
#       $jPictures[] = $picture->toJson();
#     }
#
#     $response->addData('pictures', $jPictures);
#   }
#
# //-----------------------------------------------------------------------------------------------------------------------------
#
#   private function getPicturesByUserId( &$response,
#                                           $userId )
#   {
#     $minimumClientVersion = 'V0.0.2';
#
#     // Get user
#     $em = $this->getDoctrine()->getManager();
#     $userRepository = $em->getRepository('InstaspotsUserBundle:User');
#     $user = $userRepository->findOneById($userId);
#
#     if(empty($user))
#     {
#       $response->setError('User with id \''.$userId.'\' not found');
#       return;
#     }
#
#     // Get pictures
#     $pictureRepository = $em->getRepository('InstaspotsSpotsBundle:Picture');
#     $pictureList = $pictureRepository->findByUser($user,
#                                                   array('created' => 'DESC'));
#
#     $jPictures = array();
#     foreach($pictureList as &$picture)
#     {
#       $jPictures[] = $picture->toJson();
#     }
#
#     $response->addData('pictures', $jPictures);
#   }
#
# //-----------------------------------------------------------------------------------------------------------------------------
#
#   private function getSpotsByDistance( &$response,
#                                         $latitude,
#                                         $longitude,
#                                         $maxDistance_km )
#   {
#     $minimumClientVersion = 'V0.0.2';
#
#     $repository = $this->getDoctrine()
#                          ->getManager()
#                          ->getRepository('InstaspotsSpotsBundle:Spot');
#
#     $jSpots = array();
#     foreach($repository->getArrayByDistance($latitude,
#                                             $longitude,
#                                             $maxDistance_km)
#             as &$spot)
#     {
#       $jSpot = array();
#
#       $jSpot['id']          = $spot[0]['id'];
#       $jSpot['name']        = $spot[0]['name'];
#       $jSpot['description'] = $spot[0]['description'];
#       $jSpot['latitude']    = $spot[0]['latitude'];
#       $jSpot['longitude']   = $spot[0]['longitude'];
#       $jSpot['secretSpot']  = $spot[0]['secretSpot'];
#       $jSpot['distance_km'] = (float)$spot['distance'];
#
#       $picture1 = new Picture();
#       $picture1->setId     ($spot[0]['picture1']['id']);
#       $picture1->setCreated($spot[0]['picture1']['created']);
#
#       $jSpot['pictureId1']  = $picture1->getId();
#       $jSpot['pictureUrl1'] = $picture1->getUrl();
#
#       $picture2 = new Picture();
#       $picture2->setId     ($spot[0]['picture2']['id']);
#       $picture2->setCreated($spot[0]['picture2']['created']);
#
#       if($picture2->getId() != $picture1->getId())
#       {
#         $jSpot['pictureId2']  = $picture2->getId();
#         $jSpot['pictureUrl2'] = $picture2->getUrl();
#       }
#       else
#       {
#         $jSpot['pictureId2']  = -1;
#         $jSpot['pictureUrl2'] = '';
#       }
#
#       $jSpots[] = $jSpot;
#     }
#
#     $response->addData('spots', $jSpots);
#   }
#
# //-----------------------------------------------------------------------------------------------------------------------------
#
#   private function getSpotsByUserId( &$response,
#                                       $userId )
#   {
#     $minimumClientVersion = 'V0.0.2';
#
#     // Get user
#     $em = $this->getDoctrine()->getManager();
#     $userRepository = $em->getRepository('InstaspotsUserBundle:User');
#     $user = $userRepository->findOneById($userId);
#
#     if(empty($user))
#     {
#       $response->setError('User with id \''.$userId.'\' not found');
#       return;
#     }
#
#     // Get spots
#     $spotRepository = $em->getRepository('InstaspotsSpotsBundle:Spot');
#     $spotList = $spotRepository->findByUser($user,
#                                                array('modified' => 'DESC'));
#
#     $jSpots = array();
#     foreach($spotList as &$spot)
#     {
#       $jSpots[] = $spot->toJson();
#     }
#
#     $response->addData('spots', $jSpots);
#   }
#
# //-----------------------------------------------------------------------------------------------------------------------------
# //-----------------------------------------------------------------------------------------------------------------------------
#
#   //loads up the source image, resizes it and saves with -thumb in the file name
#   private function thumb( $srcFile,
#                           $sideInPx )
#   {
#     $image = imagecreatefromjpeg($srcFile);
#     $width = imagesx($image);
#     $height = imagesy($image);
#
#     $thumb = imagecreatetruecolor($sideInPx, $sideInPx);
#
#     imagecopyresized($thumb,$image,0,0,0,0,$sideInPx,$sideInPx,$width,$height);
#
#     imagejpeg($thumb, str_replace(".jpg","-thumb.jpg",$srcFile), 85);
#
#     imagedestroy($thumb);
#     imagedestroy($image);
#   }
# }
