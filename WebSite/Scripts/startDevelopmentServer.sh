#!/bin/bash

cd "$(dirname "$0")"

DOCKER_COMPOSE_FILE='../docker-compose.yml'

docker-compose -f $DOCKER_COMPOSE_FILE down -v --remove-orphans
docker-compose -f $DOCKER_COMPOSE_FILE up -d --build

echo "Wait 3s..."
sleep 3s

docker-compose -f $DOCKER_COMPOSE_FILE exec web python manage.py migrate --noinput
docker-compose -f $DOCKER_COMPOSE_FILE exec web python manage.py createsuperuser
./fillData.sh

docker-compose -f $DOCKER_COMPOSE_FILE logs -f
