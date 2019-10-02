import jester  # ./jester/jester # i had to clone the freshest jester
import strtabs, asyncdispatch, os, strutils, json, threadpool
import lists
import pg
let pool = newAsyncPool("localhost", "gustavo", "test", "postgres", 20)
var cpucores = 4
let x: string = "test"
# var settings = newSettings(port=Port(5000), staticDir=".") #,#reusePort = true)

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

proc processLine(line: string) =
  
  settings:
    port = Port(8080)
    reusePort = true
    #appName = "/foo"
    bindAddr = "0.0.0.0"
  
  routes:
    get "/":
      resp("Hello World!")
    
    get "/db2":
        var j_data = await exec_query_async()
        resp $(%*j_data), "application/json"
    
    get "/json":
      resp("Please use POST-method on this path.")
    
    get "/redirect":
      redirect "http://192.168.0.1:80"
    
    post "/json":
      var push = parseJson($request.body)
      resp ("You have just sent this JSON: " & $push)
    
    post "/redirect":
      var push = parseJson($request.body)
      redirect "http://192.168.0.1:80"
  
  runForever()

var i = 1
while i <= cpucores:
  spawn processLine(x)
  inc(i)
sync()