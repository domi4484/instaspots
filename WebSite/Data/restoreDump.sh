#!/bin/bash

DUMP_FILE=${1}


docker exec -i postgis-lowerspot /bin/bash -c "PGPASSWORD=secret psql --set ON_ERROR_STOP=on --username postgres postgres" << EOF
DROP SCHEMA IF EXISTS tiger CASCADE;
DROP SCHEMA IF EXISTS tiger_data CASCADE;
DROP SCHEMA IF EXISTS topology CASCADE;
EOF

docker exec -i postgis-lowerspot /bin/bash -c "PGPASSWORD=secret psql --set ON_ERROR_STOP=on --username postgres postgres" < ${DUMP_FILE}

