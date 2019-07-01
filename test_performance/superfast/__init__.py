from sanic import Sanic
from sanic.response import json

from sanic import Sanic, response


app = Sanic(__name__)


@app.route("/text")
def handler_text(request):
    return response.text("Hello")


@app.route("/")
def handler_json(request):
    return response.json({"foo": "bar"})
