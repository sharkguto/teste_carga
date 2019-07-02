use futures::IntoFuture;

use actix_web::{get, web, Error, HttpRequest};

#[get("/v1/hello/{name}")]
pub fn index(req: HttpRequest, name: web::Path<String>) -> String {
    //println!("REQ: {:?}", req);
    format!("Hello sync: {}!\r\n", name)
}

pub fn index_async(_req: HttpRequest,name: web::Path<String>) -> impl IntoFuture<Item = String, Error = Error> {
//impl IntoFuture<Item = &'static str, Error = Error> {
    //println!("REQ: {:?}", req);
    Ok(format!("Hello async: {}!\r\n", name))
}

#[get("/")]
pub fn no_params() -> &'static str {
    "Hello world!\r\n"
}