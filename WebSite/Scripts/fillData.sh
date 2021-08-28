#!/bin/bash

cd "$(dirname "$0")"

cat spots.sql | docker-compose -f ../docker-compose.yml exec -T db psql -U postgres -d postgres

