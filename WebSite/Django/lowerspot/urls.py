from django.conf.urls import patterns, include, url
from django.contrib import admin

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'lowerspot.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^spots/',                    include('spots.urls')     ),
    url(r'^webservice/',               include('webservice.urls')),
    url(r'^admin/',                    include(admin.site.urls)  ),

    # rest_framework autenticazione
    url(r'^webservice/authentication/', include('rest_framework.urls', namespace='rest_framework')),
)
