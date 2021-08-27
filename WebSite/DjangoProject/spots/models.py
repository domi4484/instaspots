#from __future__ import unicode_literals

from django.contrib.gis.db import models
from django.contrib.gis.geos import Point
from django.utils import timezone
from django.contrib.auth.models import User

from datetime import datetime

class Spot(models.Model):
    id           = models.AutoField(primary_key=True)
    created      = models.DateTimeField (default = timezone.now)
    modified     = models.DateTimeField (default = timezone.now)
    user         = models.ForeignKey    (User, null=True, blank=True, on_delete=models.SET_NULL, related_name='spots')
    name         = models.CharField     (max_length=255)
    description  = models.TextField     (default="",      blank=True)
    secretspot   = models.BooleanField  (default=False)
    score        = models.IntegerField  (default=0)
    picture1_id  = models.IntegerField  (default=0)
    picture2_id  = models.IntegerField  (default=0)

    position = models.PointField(default=Point(0, 0))

    def __str__(self):
        return self.name


class Picture(models.Model):
    id        = models.AutoField(primary_key=True)
    created   = models.DateTimeField(default = timezone.now)
    user      = models.ForeignKey(User, null=True,  blank=True,  on_delete=models.SET_NULL, related_name='pictures')
    spot      = models.ForeignKey(Spot, null=False, blank=False, on_delete=models.CASCADE,  related_name='pictures')
    published = models.BooleanField(default=True)
    # likers      = models.ForeignKey    ()

    position = models.PointField(default=Point(0, 0))

    def __str__(self):
        return "picture of " + self.spot.__str__()

    def path(self):
        return 'pictures/{}/{}.jpg'.format(self.created.strftime('%Y/%m/%d'), self.id)

    def url(self):
        return 'media/{}'.format(self.path())

    def saveUploadedPicture(self, imageFile):
        savePath = self.path()
        with open(savePath, 'wb+') as destinationFile:
            for chunk in imageFile.chunks():
                destinationFile.write(chunk)

    #     // Check for valid picture
    #     if(   $uploadedFile->isValid() == false
    #        || $uploadedFile->getMimeType() != 'image/jpeg')
    #     {
    #       $response->setError('Invalid file received');
    #       return;
    #     }
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
