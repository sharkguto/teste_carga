FROM postgres:11-alpine

COPY devops/sql-scripts/dump-postgres.sql /docker-entrypoint-initdb.d/
