import options, asyncdispatch, httpbeast, json
import pg

let pool = newAsyncPool("localhost", "gustavo", "test", "postgres", 80) 

proc getPool() : AsyncPool =
    return &pool

proc onRequest(req: Request) {.async, gcsafe.} =
    
    if req.httpMethod == some(HttpGet):
        case req.path.get()
        of "/db2":
            let pp = getPool()
            let rows = $(%* await pp.rows(
                    sql"SELECT salary,address,age,id,name FROM test.company", @[]) )
            
            #var data = {"message": "rows"})

            const headers = "Content-Type: application/json"
            req.send(Http200, rows, headers)
        of "/plaintext":
            var data = $(%*{"message": "Hello, World!"})
            const headers = "Content-Type: application/json"
            req.send(Http200, data, headers)
        else:
            req.send(Http404)


run(onRequest)



######################################################
############ pure httpserver #########################
######################################################
# import asyncdispatch, asynchttpserver
# import json

# proc main =
#     var server = newAsyncHttpServer()
#     proc cb(req: Request) {.async.} =
#         var data = $(%*{"message": "Hello, World!"})
#         let headers = {"Content-Type": "application/json"}
#         await req.respond(Http200, data, headers.newHttpHeaders())

#     asyncCheck server.serve(Port(8080), cb)
#     runForever()

# main()

#ulimit -n 63536
