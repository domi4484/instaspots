
from django.conf.urls import url
from rest_framework.urlpatterns import format_suffix_patterns
from rest_framework.authtoken import views
from webservice import views as webserviceViews

urlpatterns = [
    url(r'^token-auth/$',              views.obtain_auth_token),
    url(r'^users/$',                   webserviceViews.UserList.as_view()),
    url(r'^users/(?P<pk>[0-9]+)/$',    webserviceViews.UserDetail.as_view()),
    url(r'^spots/$',                   webserviceViews.SpotList.as_view()),
    url(r'^spots/(?P<pk>[0-9]+)/$',    webserviceViews.SpotDetail.as_view()),
    url(r'^spots/byDistance/$',        webserviceViews.SpotListByDistance.as_view()),
    url(r'^spots/byUser/(?P<user>[0-9]+)/$', webserviceViews.SpotListByUser.as_view()),
    url(r'^pictures/$',                webserviceViews.PictureList.as_view()),
    url(r'^pictures/(?P<pk>[0-9]+)/$', webserviceViews.PictureDetail.as_view()),
    url(r'^pictures/byNewest/$',       webserviceViews.PictureListByNewest.as_view()),
    url(r'^webservice/$',              webserviceViews.Webservice.as_view()),
]

urlpatterns = format_suffix_patterns(urlpatterns)
