FROM redis:5-alpine
COPY devops/config/redis.conf /usr/local/etc/redis/redis.conf
#RUN sysctl vm.overcommit_memory=1
CMD [ "redis-server", "/usr/local/etc/redis/redis.conf" ]