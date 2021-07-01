#!/bin/bash

export DJANGO_SETTINGS_MODULE='lowerspot.settingsProduction'

daphne -p 2811 lowerspot.asgi:application

