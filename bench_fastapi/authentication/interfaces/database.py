#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# database.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   :
# @Date   : 12/13/2019, 12:08:52 PM

from databases import Database
from authentication.config import SQLALCHEMY_DATABASE_URI, POSTGRES_POOL_SIZE

database = Database(SQLALCHEMY_DATABASE_URI, min_size=1, max_size=POSTGRES_POOL_SIZE)


# # Execute
# query = notes.insert()
# values = {"text": "example1", "completed": True}
# await database.execute(query=query, values=values)

# # Execute many
# query = notes.insert()
# values = [
#     {"text": "example2", "completed": False},
#     {"text": "example3", "completed": True},
# ]
# await database.execute_many(query=query, values=values)
