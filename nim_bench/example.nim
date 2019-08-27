import asyncdispatch, jester, os, strutils
import json
import pg
#import db_postgres # -> use libpq
import lists


#let pool = newAsyncPool("localhost", "gustavo", "test", "postgres", 20)
let db = newAsyncPool("localhost", "gustavo", "test", "postgres", 10)

#let db = open("localhost", "gustavo", "test", "postgres")

type
    Row = object
        salary: float
        address: string
        age: int
        id: int
        name: string


proc rowizer(rec: seq[string]): JsonNode =
    result = %*Row(salary: parseFloat(rec[0]), address: rec[1],
            age: parseInt(rec[2]), id: parseInt(rec[3]), name: rec[4])

proc exec_query(): seq[JsonNode] =

    var
        j: seq[JsonNode]

    for rec in db.fastRows(sql"SELECT salary,address,age,id,name FROM test.company"):
        let row_data = rowizer(rec)
        j.add(row_data)

    result = j

router myrouter:
    get "/json":
        var j_data = exec_query()
        resp $(%*j_data), "application/json"
        #resp $(%*{"message": "Hello, World!"}), "application/json"

proc main() =

    let port = 8080.Port #paramStr(1).parseInt().Port
    let settings = newSettings(port = port)
    var jester = initJester(myrouter, settings = settings)

    jester.serve()

when isMainModule:
    main()

#nim c -d:release --threads:on --opt:speed --stackTrace:off example.nim
