'''
Created on 31 de jul de 2017

@author: gustavo
'''

from sanic import Sanic, response
from sanic.config import Config
# from sanic.exceptions import RequestTimeout,NotFound

from asyncpg import create_pool
from sanic.log import LOGGING_CONFIG_DEFAULTS
from pprint import pprint

DB_CONFIG = {
    'host': '127.0.0.1',
    'user': 'gustavo',
    'password': 'test',
    'port': '5432',
    'database': 'test'
}

Config.REQUEST_TIMEOUT = 20

app = Sanic(__name__)


def jsonify(records):
    """
    Parse asyncpg record response into JSON format
    """
    # print(records)
    list_return = []
    
    for r in records:
        itens = r.items()
        list_return.append({i[0]:i[1].rstrip() if type(i[1]) == str else i[1] for i in itens})
    return list_return    
    # return [dict(r.items()) for r in records]


@app.route("/db2", strict_slashes=True)
async def databaselocao2(request):
    pool = request.app.config['pool']
    async with pool.acquire() as conn:
        results = await conn.fetch('SELECT salary,address,age,id,name FROM test.company')
    # return response.json({'posts': results})
    return response.json({'posts': jsonify(results)})


@app.listener('before_server_start')
async def register_db(app, loop):
    # Create a database connection pool
    app.config['pool'] = await create_pool(**DB_CONFIG, loop=loop, max_size=25)


#pprint(LOGGING_CONFIG_DEFAULTS)
#LOGGING_CONFIG_DEFAULTS['loggers']['sanic.access']['level'] = 'ERROR'



# log_config=Config.LOGGING,


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8001 , workers=4, debug=None, access_log=False)
    
# ab -c100 -n10000 http://127.0.0.1:8000/

