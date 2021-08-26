docker-compose build
docker-compose up -d
docker-compose up -d --build
docker-compose logs -f
docker-compose exec web python manage.py migrate --noinput
docker-compose exec db psql --username=postgres --dbname=postgres
docker-compose down -v
