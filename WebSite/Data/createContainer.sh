#!/bin/bash

# Start postgis container
docker run --name postgis-lowerspot -e POSTGRES_PASSWORD=secret -p 5433:5432 -d postgis/postgis


