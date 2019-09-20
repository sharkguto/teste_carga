import options, asyncdispatch, json , os, strutils
import pg
import httpbeast
let pool = newAsyncPool("localhost", "gustavo", "test", "postgres", 20)

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

proc exec_query_async(): Future[seq[JsonNode]] {.async.} =
    var
        j: seq[JsonNode]
        #j {.threadvar.}: seq[JsonNode]

    let rows = await pool.rows(sql"SELECT salary,address,age,id,name FROM test.company",
            @[])
    for rec in rows:
        let row_data = await rowizer_async(rec)
        j.add(row_data)

    result = j

proc onRequest(req: Request): Future[void] {.async.} =
  if req.httpMethod == some(HttpGet):
    case req.path.get()
    of "/json":
      #var data = $(%*{"message": "Hello, World!"})
      const headers = "Content-Type: application/json"
      var data = await exec_query_async()
      req.send(Http200, $(data), headers)
    of "/plaintext":
      const data = "Hello, World!"
      const headers = "Content-Type: text/plain"
      req.send(Http200, data, headers)
    else:
      req.send(Http404)

run(onRequest)


#nim c -d:release --threads:on --opt:speed --stackTrace:off example.nim