FROM postgres:10-alpine

COPY devops/sql-scripts/test.sql /docker-entrypoint-initdb.d/
