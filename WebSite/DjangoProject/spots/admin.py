
from django.contrib.gis import admin

from .models import Spot
from .models import Picture

admin.site.register(Spot, admin.OSMGeoAdmin)
admin.site.register(Picture, admin.OSMGeoAdmin)
