import asyncdispatch, jester, strutils
import json
import pg
#import db_postgres # -> use libpq
# import lists, os
# import threadpool
#nim c -d:release --threads:off --opt:speed --stackTrace:off example.nim


let pool = newAsyncPool("localhost", "gustavo", "test", "postgres", 20)
    #let db = newAsyncPool("localhost", "gustavo", "test", "postgres", 10)

    #let db = open("localhost", "gustavo", "test", "postgres")

type
    Row = object
        salary: float
        address: string
        age: int
        id: int
        name: string



proc rowizer_async(rec: seq[string]): Future[JsonNode] {.async.} =
    result = %*Row(salary: parseFloat(rec[0]), address: rec[1],
            age: parseInt(rec[2]), id: parseInt(rec[3]), name: rec[4])

proc rowizer(rec: seq[string]): JsonNode =
    result = %*Row(salary: parseFloat(rec[0]), address: rec[1],
            age: parseInt(rec[2]), id: parseInt(rec[3]), name: rec[4])

proc exec_query_async(): Future[seq[JsonNode]] {.async.} =
    var
        j : seq[JsonNode] 
        #j {.threadvar.}: seq[JsonNode]

    let rows = await pool.rows(sql"SELECT salary,address,age,id,name FROM test.company",
            @[])
    for rec in rows:
        let row_data = await rowizer_async(rec)
        j.add(row_data)

    result = j

proc exec_query(): seq[JsonNode] =
    let db = open("localhost", "gustavo", "test", "postgres")
    var
        j: seq[JsonNode]

    for rec in db.fastRows(sql"SELECT salary,address,age,id,name FROM test.company"):
        let row_data = rowizer(rec)
        j.add(row_data)

    db.close()
    result = j

router myrouter:
    # get "/db2":
    #     var j_data = await exec_query_async()
    #     resp $(%*j_data), "application/json" 
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

#ulimit -n 63536
#nim c -d:release --threads:off --opt:speed --stackTrace:off example.nim -> for async
#nim c -d:release --threads:on --opt:speed --stackTrace:off --gc:boehm example.nim
#nim c -d:release --threads:on --opt:speed --stackTrace:off --showAllMismatches:on example.nim
#nimble install jester@#head
#nimble install pg@#head
#nimble install print@#head

#nim c -d:release --threads:on --opt:speed --stackTrace:on --threadAnalysis:off example.nim
#wrk -c 4096 -t 4 http://localhost:8080/json -d 15 --latency