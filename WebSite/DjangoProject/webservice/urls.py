from django.conf.urls import url
from rest_framework.urlpatterns import format_suffix_patterns
from webservice import views

urlpatterns = [
    url(r'^users/$',                   views.UserList.as_view()),
    url(r'^users/(?P<pk>[0-9]+)/$',    views.UserDetail.as_view()),
    url(r'^spots/$',                   views.SpotList.as_view()),
    url(r'^spots/(?P<pk>[0-9]+)/$',    views.SpotDetail.as_view()),
    url(r'^spots/byDistance/$',        views.SpotListByDistance.as_view()),
    url(r'^pictures/$',                views.PictureList.as_view()),
    url(r'^pictures/(?P<pk>[0-9]+)/$', views.PictureDetail.as_view()),
    url(r'^pictures/byNewest/$',       views.PictureListByNewest.as_view()),
    url(r'^webservice/$',              views.Webservice.as_view()),
]

urlpatterns = format_suffix_patterns(urlpatterns)
