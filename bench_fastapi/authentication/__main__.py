#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# __main__.py
# @Author : Gustavo Freitas (gustavo@gmf-tech.com)
# @Link   :
# @Date   : 12/12/2019, 5:46:52 PM

from . import app
import uvicorn

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8080)

