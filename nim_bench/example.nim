# import xander

# get "/":
#   respond "Hello World!"

# runForever(8080)


import json

import jester, asyncdispatch

settings:
  port = Port(8080)


routes:
  get "/json":
    var data = $(%*{"message": "Hello, World!"})
    resp data, "application/json"

  get "/plaintext":
    const data = "Hello, World!"
    resp data, "text/plain"


runForever()

#nim c -d:release --threads:on example.nim
#sudo sysctl -w fs.inotify.max_user_watches=100000

#ulimit -n 63536
