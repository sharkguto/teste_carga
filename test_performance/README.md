
# flask-connexion

``` bash 
gunicorn   --workers=8 
gunicorn wsgi:app --workers=8 -b "0.0.0.0:8080" --worker-class=meinheld.gmeinheld.MeinheldWorker --chdir load_server/
```

gunicorn wsgi_async:app --workers=8 -b "0.0.0.0:8080" --worker-class=aiohttp.worker.GunicornWebWorker --chdir load_server/


```
gustavo@Inspiron-7572:~/git-repos/test_performance$ django-admin startapp hello
gustavo@Inspiron-7572:~/git-repos/test_performance$ django-admin startproject djteste
gustavo@Inspiron-7572:~/git-repos/test_performance$ cd djteste/
gustavo@Inspiron-7572:~/git-repos/test_performance/djteste$ django-admin startapp hello
gustavo@Inspiron-7572:~/git-repos/test_performance/djteste$ ```



## django

```bash
python3 manage.py runserver 127.0.1:8080
gunicorn wsgi:application --workers=8 -b "0.0.0.0:8080" --worker-class=meinheld.gmeinheld.MeinheldWorker --chdir djteste
```

## criar modulos

```bash
python3 manage.py startapp calc
```

## sanic

gunicorn sanic_test:app --workers=8 -b "0.0.0.0:8080" --worker-class=sanic.worker.GunicornWorker --log-level error

gunicorn myapp:app --bind 0.0.0.0:1337 --worker-class sanic.worker.GunicornWorker

## starlette

gunicorn star_test:app --workers=8 -b "0.0.0.0:8080" --worker-class=uvicorn.workers.UvicornWorker --log-level error


gunicorn star_test:app --workers=8 -b "0.0.0.0:8080" --worker-class=uvicorn.workers.UvicornWorker

gunicorn star_test:app --workers=8 -b "0.0.0.0:8080" --worker-class=aiohttp.worker.GunicornUVLoopWebWorker

hypercorn star_test:app -k uvloop --workers=8 -b "0.0.0.0:8080"


uwsgi --http :8080 --wsgi-file load_server/wsgi_async.py --master --processes 8

uwsgi --http :8080 --wsgi-file load_server/wsgi_async.py --master --processes 8 -l 128 --disable-logging


gunicorn load_server3:application --workers=4 -b "0.0.0.0:8080" --worker-class=aiohttp.UvicornWorker --chdir load_server3/

gunicorn load_server3:application --workers=8 -b "0.0.0.0:8080" --worker-class=aiohttp.worker.GunicornWebWorker --chdir load_server3/

gunicorn load_server3:application --workers=8 -b "0.0.0.0:8080" --worker-class=aiohttp.GunicornUVLoopWebWorker --chdir load_server3/


sudo pip3 install git+https://github.com/aio-libs/aiohttp.git

sudo apt install libwebsockets-dev 


--workers 8