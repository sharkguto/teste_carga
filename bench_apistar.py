from apistar import App, Route
from psycopg2 import pool
import logging

DB_CONFIG = {
    'host': '127.0.0.1',
    'user': 'gustavo',
    'password': 'test',
    'port': '5432',
    'database': 'test'
}

connection_pool = pool.ThreadedConnectionPool(10, 80, **DB_CONFIG)
conn = connection_pool.getconn()


def db2(name=None):
    with conn.cursor() as cur:
        cur.execute('SELECT salary,address,age,id,name FROM test.company')
        results = cur.fetchall()
    return {'posts': jsonify(results)}


def jsonify(records):
    """
    Parse asyncpg record response into JSON format
    """

    list_return = []
    list_keys = ['salary', 'address', 'age', 'id', 'name']
    for r in records:

        itens = [i for i in r]
        itens = zip(list_keys, itens)
        list_return.append({i[0]: i[1].rstrip() if type(
            i[1]) == str else i[1] for i in itens})
    return list_return


routes = [
    Route('/db2', method='GET', handler=db2),
]

app = App(routes=routes)

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

if __name__ == '__main__':
    app.serve('127.0.0.1', 5000, use_debugger=False, use_reloader=False)
