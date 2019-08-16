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


#sudo sysctl net.core.somaxconn=9086
#sudo sysctl net.core.netdev_max_backlog=4096
#sudo sysctl net.ipv4.tcp_max_syn_backlog=4096
#sudo ifconfig enp3s0 txqueuelen 10000
#sudo sysctl -p

#sudo sysctl net.ipv4.tcp_fin_timeout=15
#sudo sysctl net.ipv4.ip_local_port_range="15000 61000"

#wrk -c 2046 -t 4 http://192.168.0.134:8080/json -d 400

# sudo sysctl net.ipv4.tcp_tw_recycle=1
# sudo sysctl net.ipv4.tcp_tw_reuse=1 