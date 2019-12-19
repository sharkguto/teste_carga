#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# config.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   :
# @Date   : 12/12/2019, 12:13:24 PM

import os

API_V1_STR = "/v1"
POSTGRES_SERVER = os.getenv("DB_SERVER", "localhost")
POSTGRES_USER = os.getenv("DB_USER", "postgres")
POSTGRES_PASSWORD = os.getenv("DB_PASSWORD", "test")
POSTGRES_DB = os.getenv("DB_NAME", "postgres")
POSTGRES_POOL_SIZE = int(os.getenv("POSTGRES_POOL_SIZE", 10))
SQLALCHEMY_DATABASE_URI = (
    f"postgresql://{POSTGRES_USER}:{POSTGRES_PASSWORD}@{POSTGRES_SERVER}/{POSTGRES_DB}"
)
ASYNC_DATABASE_URI = f"postgresql://{POSTGRES_USER}:{POSTGRES_PASSWORD}@{POSTGRES_SERVER}/{POSTGRES_DB}?min_size=1&max_size={POSTGRES_POOL_SIZE}"
