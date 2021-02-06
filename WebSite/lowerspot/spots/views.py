from django.shortcuts import render
from django.template import loader
from django.http import HttpResponse, Http404

from .models import Spot, Picture
from django.contrib.auth.models import User

def index(request):
    latest_picture_list = Picture.objects.order_by('-created')[:5]
    context = { 'latest_picture_list': latest_picture_list }
    return render(request, 'spots/index.html', context)

def spot(request, spot_id):
    try:
        spot = Spot.objects.get(pk=spot_id)
        picture_list = Picture.objects.filter(spot=spot).order_by('-created')
        
    except Spot.DoesNotExist:
        raise Http404("Spot does not exist")

    return render(request, 'spots/spot.html', {'spot': spot,
                                               'picture_list' : picture_list})

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
    
