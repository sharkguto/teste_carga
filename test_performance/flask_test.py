import ujson as json

from flask import Flask

app = Flask(__name__)


@app.route("/v1/hello/<name>")
def hello_world(name):
    return json.dumps({"response": f" ola {name}. bem vindo ao clube"})


if __name__ == "__main__":
    app.run(port=8080)