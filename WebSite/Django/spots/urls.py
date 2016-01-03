
from django.conf.urls import url

from . import views

urlpatterns = [
    url(r'^$',                          views.index,      name='index'     ),
    url(r'^spot/(?P<spot_id>[0-9]+)/$', views.spot,       name='spot'      ),
    url(r'^user/(?P<user_id>[0-9]+)/$', views.user,       name='user'      ),
    url(r'^download/$',                 views.download,   name='download'  ),
]
