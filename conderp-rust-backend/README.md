# rust

## diesel

```bash
sudo apt install libpq-dev libmysqlclient-dev libsqlite3-dev 
cargo install diesel_cli
cargo install diesel_cli --no-default-features --features postgres
cargo install diesel_cli --no-default-features --features "postgres sqlite mysql"
```

```bash
#generate migrate
diesel migration generate create_companies

```