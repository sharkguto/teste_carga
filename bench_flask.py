from flask import Flask
app = Flask(__name__)
from psycopg2 import pool
from flask import jsonify as r_json
DB_CONFIG = {
    'host': '127.0.0.1',
    'user': 'gustavo',
    'password': 'test',
    'port': '5432',
    'database': 'test'
}

connection_pool = pool.ThreadedConnectionPool(10, 80, **DB_CONFIG)
conn = connection_pool.getconn()
# use conn, then:
# connection_pool.putconn(conn)


@app.route('/db2')
def hello_world():
    with conn.cursor() as cur:
        cur.execute('SELECT salary,address,age,id,name FROM test.company')
        results = cur.fetchall()
        # print(results)
    return r_json({'posts': jsonify(results)})


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


import logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)


if __name__ == "__main__":
    app.run() #host="0.0.0.0", port=8001
