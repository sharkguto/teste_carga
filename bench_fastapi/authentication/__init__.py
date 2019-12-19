#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# __init__.py.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   :
# @Date   : 12/12/2019, 9:54:56 AM


from fastapi import FastAPI
from authentication.controllers import api_router
from starlette.middleware.cors import CORSMiddleware
from starlette.requests import Request
from authentication import config

from authentication.interfaces.database import database

app = FastAPI(title="authentication", openapi_url=f"{config.API_V1_STR}/openapi.json")


app.add_middleware(
    CORSMiddleware,
    allow_origins=["*.gmf-tech.com", "*.gmf-tech.com.br", "localhost"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
),

app.include_router(api_router, prefix=config.API_V1_STR)


@app.on_event("startup")
async def startup():
    await database.connect()


@app.on_event("shutdown")
async def shutdown():
    await database.disconnect()
