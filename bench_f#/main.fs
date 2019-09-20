type WorkResults =
    | DoWork of int
    | Work of float32

let dowork n =
    async {
        do printfn "work %d" n
        return DoWork(n)
    }

let work i = async {
  do! Async.Sleep(2000)
  printfn "work finished %d" i 
  return Work(float32 i / 4.0f)
}

[dowork 1; work 2]
|> Async.Parallel
|> Async.RunSynchronously
|> printf "%A"