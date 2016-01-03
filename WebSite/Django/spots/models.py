from __future__ import unicode_literals

from django.db import models
from django.contrib.auth.models import User

from datetime import datetime

class Spot(models.Model):
    created      = models.DateTimeField (default = datetime.now())
    modified     = models.DateTimeField (default = datetime.now())
    user         = models.ForeignKey    (User, null=True, blank=True, on_delete=models.SET_NULL, related_name='spots')
    name         = models.CharField     (max_length=255)
    description  = models.TextField     (default="",      blank=True)
    secret       = models.BooleanField  (default=False)
    score        = models.IntegerField  (default=0)
    latitude     = models.FloatField    (default=0.0)
    longitude    = models.FloatField    (default=0.0)
    picture_id_1 = models.IntegerField  (default=0)
    picture_id_2 = models.IntegerField  (default=0)

    def __str__(self):
        return self.name



class Picture(models.Model):
    created     = models.DateTimeField (default = datetime.now())
    user        = models.ForeignKey    (User, null=True,  blank=True,  on_delete=models.SET_NULL, related_name='pictures')
    spot        = models.ForeignKey    (Spot, null=False, blank=False, on_delete=models.CASCADE,  related_name='pictures')
    latitude    = models.FloatField    (default=0.0)
    longitude   = models.FloatField    (default=0.0)
    published   = models.BooleanField  (default=True)
    #likers      = models.ForeignKey    ()

    def __str__(self):
        return "picture of " + self.spot.__str__()

    def saveUploadedPicture(imageFile):
        savePath = 'pictures/' + self.created.strftime('%Y/%m/%d/') + self.id + '.jpg'
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
