#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# wsgi_puro.py
# @Author : Gustavo Freitas (gustavo.freitas@ies2.com.br)
# @Link   : 
# @Date   : 2/22/2019, 5:14:56 PM


# flask puro raiz

import ujson as json

from flask import Flask

app = Flask(__name__)


@app.route("/v1/hello/<name>")
def hello_world(name):
    return json.dumps({"response": f" ola {name}. bem vindo ao clube"})


if __name__ == "__main__":
    app.run(port=8080)