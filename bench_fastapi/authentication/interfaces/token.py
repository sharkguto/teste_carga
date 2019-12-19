#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# token.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   :
# @Date   : 12/18/2019, 4:54:20 PM

from fastapi import Depends, Header, HTTPException
import jwt
from authentication.interfaces.database import database


async def verify_token(x_api_key: str = Header(None)):

    if not x_api_key:
        raise HTTPException(status_code=403, detail="X-Api-Key header invalid")
    try:
        user = jwt.decode(x_api_key, verify=False)
    except Exception as ex:
        print(ex)
        raise HTTPException(status_code=403, detail="X-Api-Key header invalid")

    print(user)

    sql = """SELECT tu.token
    FROM users.tbl_users tu 
    WHERE tu.passwd is NOT NULL 
    AND tu.email = :email
    AND tu.enabled = true """

    users = await database.fetch_one(query=sql, values={"email": user["email"]})
    if not users:
        raise HTTPException(status_code=401)
    try:
        user = jwt.decode(x_api_key, users["token"], algorithms=["HS256"], verify=True)
    except Exception as ex:
        print(ex)
        raise HTTPException(status_code=401)

    return user

