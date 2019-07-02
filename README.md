# teste_carga
teste de carga



#run java
gradle bootRun

#pytest test_*.py --cov=. --cov-report=

pytest --cov=. tests/ --cov-fail-under 80 --cov-report xml --cov-report term-missing

## dump mysql

```bash
mysqldump -u root -p"devel123" -h 127.0.0.1 test > devops/sql-scripts/dump-mysql.sql
```

## dump postgresql

```bash
PGPASSWORD=test pg_dump -h 127.0.0.1 -U gustavo -F p test > dump-postgres.sql
```

tunning

```sql
shared_buffers='2GB'        # to ensure buffers our dataset
work_mem='2GB'      # increase work_mem  as it helps a lot for analytical stuff
max_parallel_workers_per_gather=2   # the default actually, so just as a reminder
shared_preload_libraries='pg_stat_statements'   # for storing/analyzing test results
logging_collector=on
```
