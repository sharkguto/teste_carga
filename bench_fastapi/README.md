# AUTHENTICATION API

## running

```bash
gunicorn authentication:app --workers=8 -b "0.0.0.0:8080" --worker-class=uvicorn.workers.UvicornWorker --log-level info
```

prod

```bash
gunicorn authentication:app --workers=8 -b "0.0.0.0:8080" --worker-class=uvicorn.workers.UvicornWorker --log-level info
```

with dotenv

```bash
dotenv -f .env run gunicorn authentication:app --workers=8 -b "0.0.0.0:8080" --worker-class=uvicorn.workers.UvicornWorker --log-level error
```

postgresql password generate

```bash
CREATE EXTENSION pgcrypto;

INSERT INTO users.tbl_users (email, passwd) VALUES (
  'gustavo@test.com',
  crypt('123456', gen_salt('bf'))
);
```


## env

```bash
## LOCAL

DB_SERVER=localhost
DB_USER=postgres
DB_PASSWORD=test
DB_NAME=postgres
POSTGRES_POOL_SIZE=10
```
