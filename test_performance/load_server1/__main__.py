from . import app


if __name__ == "__main__":
    app.run(port=8080)


# gunicorn load_server3:app --bind localhost:8080 --worker-class aiohttp.worker.GunicornWebWorker

#gunicorn load_server3:app --workers=4 -b "0.0.0.0:8080" --worker-class=aiohttp.GunicornUVLoopWebWorker --chdir load_server3/
#gunicorn load_server3:app --workers=4 -b "0.0.0.0:8080" --worker-class=aiohttp.worker.GunicornWebWorker --chdir load_server3/
#gunicorn load_server33:app --workers=4 -b "0.0.0.0:8080" --worker-class=meinheld.gmeinheld.MeinheldWorker --chdir load_server33/
