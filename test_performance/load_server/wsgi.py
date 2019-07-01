
#!/usr/bin/env python
# -*- coding: utf-8 -*-


import connexion
import sys
import os

sys.path.insert(0, f"{os.getcwd()}")


app = connexion.App(__name__, specification_dir="./swagger/")
app.add_api("swagger.yaml", arguments={"title": "teste de carga"})


if __name__ == "__main__":
    app.run()


    
#!/usr/bin/env python
# -*- coding: utf-8 -*-

# flask puro raiz

# import ujson as json

# from flask import Flask

# app = Flask(__name__)


# @app.route("/v1/hello/<name>")
# def hello_world(name):
#     return json.dumps({"response": f" ola {name}. bem vindo ao clube"})


# if __name__ == "__main__":
#     app.run(port=8080)

