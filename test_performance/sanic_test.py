from sanic import Sanic
import logging
from sanic import Sanic, response

log = logging.getLogger(__name__)


LOG_SETTINGS = {
    "version": 1,
    "disable_existing_loggers": False,
    "handlers": {
        "console": {
            "class": "logging.StreamHandler",
            "level": "ERROR",
            "formatter": "default",
        }
    },
    "formatters": {
        "default": {
            "format": "%(asctime)s %(levelname)s %(name)s:%(lineno)d | %(message)s"
        }
    },
    "loggers": {"": {"level": "ERROR", "handlers": ["console"], "propagate": True}},
}

app = Sanic(__name__, log_config=LOG_SETTINGS)


@app.route("/v1/hello/<name>")
def handler_json(request, name):
    return response.json({"response": f" ola {name}. bem vindo ao clube"})
