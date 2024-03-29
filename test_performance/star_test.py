#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# star_test.py
# @Author :  ()
# @Link   :
# @Date   : 2/20/2019, 12:57:19 PM


from starlette.applications import Starlette
from starlette.responses import JSONResponse
import uvicorn

app = Starlette()


@app.route("/v1/hello/{name}")
async def homepage(request):
    return JSONResponse(
        {"response": f' ola {request.path_params["name"]}. bem vindo ao clube'}
    )


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8080)

