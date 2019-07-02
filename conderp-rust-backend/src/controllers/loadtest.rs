use futures::IntoFuture;

use actix_web::{get, Error, HttpRequest};

#[get("/db2")]
pub fn db2() -> &'static str {
    "Hello world!\r\n"
}

pub fn db2_async(_req: HttpRequest) -> impl IntoFuture<Item = String, Error = Error> {
    Ok(format!("ok"))
}
