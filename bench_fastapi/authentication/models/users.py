#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# users.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   :
# @Date   : 12/12/2019, 12:28:43 PM

from typing import Optional, Any, List, Dict

from pydantic import BaseModel
from datetime import datetime


# Shared properties
class UserBase(BaseModel):
    email: str
    name: Optional[str]
    linkedin_id: Optional[str]


class UserBaseInDB(UserBase):
    id: int = None
    enabled: Optional[bool] = True
    created_at: Optional[datetime] = None
    updated_at: Optional[datetime] = None
    disabled_at: Optional[datetime] = None
    pwd_updated_at: Optional[datetime] = None
    passwd: Optional[str] = None


# Properties to receive via API on creation
class UserCreate(UserBaseInDB):
    email: str
    password: str


# Properties to receive via API on update
class UserUpdate(UserBaseInDB):
    passwd: Optional[str] = None


# Additional properties to return via API
class User(UserBase):
    acl: Optional[List[Dict]] = []


# Additional properties stored in DB
class UserInDB(UserBaseInDB):
    hashed_password: str
