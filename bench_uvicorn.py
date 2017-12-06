'''
Created on 27 de out de 2017

@author: gustavo
'''

#uvicorn e uvtools

from uvitools.routing import Route, Router
import ujson
from asyncpg import create_pool
import asyncio


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


async def setup_db(loop):
    return await create_pool(**DB_CONFIG,loop=loop, max_size=25)

async def hello_world(message, channels):
    content = b'Hello, world'
    response = {
        'status': 200,
        'headers': [
            [b'content-type', b'text/plain'],
        ],
        'content': content
    }
    await channels['reply'].send(response)
    
    
async def db2(message, channels):
    global app
    async with app.db.acquire() as conn:
        results = await conn.fetch('SELECT salary,address,age,id,name FROM test.company')
    json_resp = jsonify(results)
    await channels['reply'].send({
        'status': 200,
        'headers': [
            [b'content-type', b'application/json']
        ],
        'content': ujson.dumps(json_resp).encode()
    })
    
    
async def hello_user(message, channels):
    data = {'hello': message['args']['username']}
    await channels['reply'].send({
        'status': 200,
        'headers': [
            [b'content-type', b'application/json']
        ],
        'content': ujson.dumps(data).encode()
    })

app = Router([
    Route('/hello/', hello_world),
    Route('/hello/<username>/', hello_user),
    Route('/db2', db2)
])


apploop = asyncio.get_event_loop()
app.db = apploop.run_until_complete(setup_db(apploop))
