from vibora import Vibora, JsonResponse
from asyncpg import create_pool
from vibora.hooks import Events

app = Vibora()

DB_CONFIG = {
    'host': '127.0.0.1',
    'user': 'gustavo',
    'password': 'test',
    'port': '5432',
    'database': 'test'
}

# Config will be a new component.


class Config:

    def __init__(self):
        self.name = 'Vibora Component postgresql'

    def set_pool(self, pool):
        self.pool = pool


def jsonify(records):
    """
    Parse asyncpg record response into JSON format
    """
    # print(records)
    list_return = []

    for r in records:
        itens = r.items()
        list_return.append({i[0]: i[1].rstrip() if type(
            i[1]) == str else i[1] for i in itens})
    return list_return


@app.handle(Events.BEFORE_SERVER_START)
async def before_start():
    pool = await create_pool(**DB_CONFIG, max_size=20)
    cfg = Config()
    cfg.set_pool(pool)
    app.components.add(cfg)


@app.route('/db2', methods=['GET'])
async def home(postg: Config):
    async with postg.pool.acquire() as conn:
        results = await conn.fetch('SELECT salary,address,age,id,name FROM test.company')

    return JsonResponse(results)

if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0', port=5000, workers=4)
