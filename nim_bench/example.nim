import asyncdispatch, jester, os, strutils
import json
#import pg
import db_postgres # -> use libpq


#let pool = newAsyncPool("localhost", "gustavo", "test", "postgres", 20)
let db = open("localhost", "gustavo", "test", "postgres")

router myrouter:
  get "/json":
    const rows = db.fastRows(sql"SELECT 1")
    for row in rows:
      echo row
    resp $(%*{"message": "Hello, World!"}), "application/json"

proc main() =
  
  let port = 8080.Port #paramStr(1).parseInt().Port
  let settings = newSettings(port = port)
  var jester = initJester(myrouter, settings = settings)
  jester.serve()

when isMainModule:
  main()

# import json

# import jester, asyncdispatch
# import pg
# #import db_postgres # -> use libpq

# #let db = open("localhost", "gustavo", "test", "postgres")
# let pool = newAsyncPool("localhost", "gustavo", "test", "postgres", 20)

# settings:
#   port = Port(8080)


# routes:
#   get "/json":
#     let rows = waitFor pg.rows(sql"SELECT salary,address,age,id,name FROM test.company")

#     var data = $(%*{"message": "Hello, World!"})
#     resp data, "application/json"

#   get "/plaintext":
#     const data = "Hello, World!"
#     resp data, "text/plain"


runForever()

#nim c -d:release --threads:on example.nim
#sudo sysctl -w fs.inotify.max_user_watches=100000

#ulimit -n 63536


#sudo sysctl net.core.somaxconn=9086
#sudo sysctl net.core.netdev_max_backlog=4096
#sudo sysctl net.ipv4.tcp_max_syn_backlog=4096
#sudo ifconfig enp3s0 txqueuelen 10000
#sudo sysctl -p

#sudo sysctl net.ipv4.tcp_fin_timeout=15
#sudo sysctl net.ipv4.ip_local_port_range="15000 61000"

#wrk -c 2046 -t 4 http://0.0.0.0:8080/json -d 400
