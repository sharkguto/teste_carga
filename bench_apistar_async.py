from apistar import ASyncApp, Route
from asyncpg import create_pool
import logging
from apistar.events import on_start
from apistar.server.components import Component

DB_CONFIG = {
    'host': '127.0.0.1',
    'user': 'gustavo',
    'password': 'test',
    'port': '5432',
    'database': 'test'
}


class Database:

    def __init__(self):
        on_start(self.connect)

    async def connect(self):
        self.pool = await create_pool(**DB_CONFIG, max_size=20)


class DatabaseComponent(Component):

    def resolve(self) -> Database:
        return Database()


async def db2(db: Database):
    async with db.pool.acquire() as conn:
        results = await conn.fetch(
            'SELECT salary,address,age,id,name FROM test.company')
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

app = ASyncApp(routes=routes)

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

if __name__ == '__main__':
    app.serve('127.0.0.1', 5000, use_debugger=False, use_reloader=False)
