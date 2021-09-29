
# Django imports
from django.shortcuts import render
from django.template import loader
from django.http import HttpResponse, Http404
from django.core.files.storage import FileSystemStorage
from django.contrib.auth.models import User

# Project imports
from .models import Spot, Picture


def index(request):
    latest_picture_list = Picture.objects.order_by('-created')[:5]
    context = {'latest_picture_list': latest_picture_list}
    return render(request, 'spots/index.html', context)

def map(request):
    return render(request, 'spots/map.html', {})

def spot(request, spot_id):
    try:
        spot = Spot.objects.get(pk=spot_id)
        picture_list = Picture.objects.filter(spot=spot).order_by('-created')

    except Spot.DoesNotExist:
        raise Http404("Spot does not exist")

    return render(request, 'spots/spot.html', {'spot': spot,
                                               'picture_list': picture_list})

def user(request, user_id):
    try:
        user = User.objects.get(pk=user_id)
    except User.DoesNotExist:
        raise Http404("User does not exist")

    picture_list = Picture.objects.filter(user=user).order_by('-created')

    return render(request, 'spots/user.html', {'user'         : user,
                                               'picture_list' : picture_list})

def download(request):
    return render(request, 'spots/download.html')

def image_upload(request):
    if request.method == "POST" and request.FILES["image_file"]:
        image_file = request.FILES["image_file"]
        fs = FileSystemStorage()
        filename = fs.save(image_file.name, image_file)
        image_url = fs.url(filename)
        print(image_url)
        return render(request, "spots/upload.html", {
            "image_url": image_url
        })

    return render(request, "spots/upload.html")
    
