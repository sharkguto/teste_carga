#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# login.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   :
# @Date   : 12/12/2019, 11:43:07 AM
from typing import Optional, Any
from fastapi import APIRouter, Body, Depends, HTTPException
from fastapi import Header, Security
from authentication.models.users import User
from fastapi.security import HTTPBasic, HTTPBasicCredentials, APIKeyHeader
from typing import List
from starlette.responses import Response
from fastapi.encoders import jsonable_encoder
from authentication.interfaces.database import database
import jwt
from starlette.status import HTTP_400_BAD_REQUEST, HTTP_401_UNAUTHORIZED
from datetime import datetime, timedelta
from hashlib import sha256
from authentication.interfaces.token import verify_token

router = APIRouter()
security = HTTPBasic(auto_error=True)
api_key = APIKeyHeader(name="x-api-key", auto_error=True)


@router.post("/login", tags=["token"])
async def renew_token(
    response: Response,
    user: dict = Depends(verify_token),
    x_api_key: str = Header(None),
):
    response.headers["x-api-key"] = x_api_key
    return {"verified": True, "user": user["email"]}


@router.put("/login", tags=["token"])
async def renew_token(response: Response, user: dict = Depends(verify_token)):

    sql = """UPDATE users.tbl_users 
    SET token = :token WHERE 
    id = :id"""

    token = f"{user['pwd_updated_at']}-{user['email']}-{datetime.now()}"

    mhash = sha256(token.encode("utf-8"))

    token = mhash.hexdigest()

    await database.execute(query=sql, values={"id": user["id"], "token": token})

    response.headers["x-api-key"] = jwt.encode(
        {**user, **dict(exp=(datetime.now() + timedelta(hours=8)))},
        token,
        algorithm="HS256",
    ).decode()

    return {"renew": True}


# @router.post("/login", dependencies=[Depends(verify_token)])
# async def renew_token(x_api_key: str = Header(None)):
#    return {"ok": x_api_key}


@router.get(
    "/login", response_model=User, tags=["auth"], response_model_exclude_unset=True
)
async def login_basic(
    response: Response, authorization: HTTPBasicCredentials = Security(security)
):

    sql = """SELECT tu.id, tu.email, tu."name", tu.linkedin_id , tu.pwd_updated_at
    FROM users.tbl_users tu 
    WHERE tu.passwd is NOT NULL 
    AND tu.passwd = crypt(:secret,tu.passwd) 
    AND tu.email = :email
    AND tu.enabled = true """

    users = await database.fetch_one(
        query=sql,
        values={"email": authorization.username, "secret": authorization.password},
    )
    if not users:
        raise HTTPException(status_code=HTTP_401_UNAUTHORIZED)

    user = jsonable_encoder(users)

    sql = """SELECT tp.acl_profile as profile
    FROM users.tbl_users tu inner join 
  	users.tbl_profile_users tpu on tpu.id_users = tu.id inner join 
  	users.tbl_profile tp on tp.id = tpu.id_profile
    WHERE tu.passwd is NOT NULL 
    AND tu.passwd = crypt(:secret,tu.passwd) 
    AND tu.email = :email"""

    profiles = await database.fetch_all(
        query=sql,
        values={"email": authorization.username, "secret": authorization.password},
    )
    if not profiles:
        raise HTTPException(status_code=HTTP_401_UNAUTHORIZED)

    user["acl"] = jsonable_encoder(profiles)

    sql = """UPDATE users.tbl_users 
    SET token = :token WHERE 
    id = :id"""

    token = f"{user['pwd_updated_at']}-{authorization.username}-{datetime.now()}"

    mhash = sha256(token.encode("utf-8"))

    token = mhash.hexdigest()

    await database.execute(query=sql, values={"id": user["id"], "token": token})

    response.headers["x-api-key"] = jwt.encode(
        {**user, **dict(exp=(datetime.now() + timedelta(hours=8)))},
        token,
        algorithm="HS256",
    ).decode()

    return user

