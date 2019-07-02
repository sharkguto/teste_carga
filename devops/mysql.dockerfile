# Derived from official mysql image (our base image)
FROM mysql:5.6

# Add a database
ENV MYSQL_DATABASE test

# Add the content of the sql-scripts/ directory to your image
# All scripts in docker-entrypoint-initdb.d/ are automatically
# executed during container startup
COPY devops/sql-scripts/dump-mysql.sql /docker-entrypoint-initdb.d/