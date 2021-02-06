from rest_framework import serializers
from spots.models import Spot
from spots.models import Picture
from django.contrib.auth.models import User

class UserSerializer(serializers.ModelSerializer):
    spots = serializers.PrimaryKeyRelatedField(many=True, queryset=Spot.objects.all())

    class Meta:
        model = User
        fields = ('id', 'username', 'spots')


class SpotSerializer(serializers.ModelSerializer):
    user = serializers.CharField(source='user.username', read_only=True)

    class Meta:
        model = Spot
        fields = ('id', 'user', 'name', 'description')


class PictureSerializer(serializers.ModelSerializer):
    user = serializers.CharField(source='user.username', read_only=True)

    class Meta:
        model = Picture
        fields = ('id', 'created', 'user', 'spot', 'latitude', 'longitude', 'published')
