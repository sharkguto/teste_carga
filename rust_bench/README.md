#

## dependencies OS

```bash
sudo apt install libpq-dev libmysqlclient-dev libsqlite3-dev
```

## rust

```bash
cargo install diesel_cli --no-default-features --features "postgres sqlite mysql"
```

```bash
cargo new bench_actix --bin
```
