from django.urls import path

from . import views

urlpatterns = [
    path('',                    views.index, name='index'),
    path('map/',                views.map,   name='map'),
    path('<int:spot_id>/',      views.spot,  name='spot'),
    path('user/<int:user_id>/', views.user,  name='user'),
    path('upload/',             views.image_upload, name='upload'),
]
