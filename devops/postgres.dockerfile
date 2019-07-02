FROM postgres:11

ENV POSTGRES_DB test

COPY devops/sql-scripts/dump-postgres.sql /docker-entrypoint-initdb.d/