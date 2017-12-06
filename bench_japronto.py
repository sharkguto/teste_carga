'''
Created on 26 de out de 2017

@author: gustavo
'''

import random

import ujson
from japronto import Application
from asyncpg import create_pool

DB_CONFIG = {
    'host': '127.0.0.1',
    'user': 'gustavo',
    'password': 'teste',
    'port': '5432',
    'database': 'test'
}

def jsonify(records):
    """
    Parse asyncpg record response into JSON format
    """
    #print(records)
    list_return = []
    
    for r in records:
        itens = r.items()
        list_return.append({i[0]:i[1].rstrip() if type(i[1])==str else i[1] for i in itens})
    return list_return    


def index(request):
    data = {'message': 'hello world'}
    body = ujson.dumps(data)
    return request.Response(text=body)

async def setup_db(loop):
    return await create_pool(**DB_CONFIG,loop=loop, max_size=25)

async def db(request):
    async with request.app.db.acquire() as conn:
        results = await conn.fetch('SELECT salary,address,age,id,name FROM test.company')
    #return response.json({'posts': results})
    body = ujson.dumps({'posts': jsonify(results)})
    return request.Response(text=body, headers={'Content-Type': 'application/json'})




app = Application()
app.db = app.loop.run_until_complete(setup_db(app.loop))
app.router.add_route('/', index)
app.router.add_route('/db2', db)
app.run(port=8001)#,worker_num=4)  


