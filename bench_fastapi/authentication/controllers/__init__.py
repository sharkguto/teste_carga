#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# __init__.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   : 
# @Date   : 12/12/2019, 11:43:32 AM

from fastapi import APIRouter

from authentication.controllers import login

api_router = APIRouter()
api_router.include_router(login.router, tags=["login"])


# api_router.include_router(users.router, prefix="/users", tags=["users"])
# api_router.include_router(utils.router, prefix="/utils", tags=["utils"])
# api_router.include_router(items.router, prefix="/items", tags=["items"])