#!/bin/bash

docker exec -i postgis-lowerspot /bin/bash -c "PGPASSWORD=secret pg_dump --username postgres -d postgres" > dump.sql

