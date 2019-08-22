use actix_web::{get, web, Error, HttpRequest};
use futures::IntoFuture;
use std::env;

#[get("/v1/hello/{name}")]
pub fn index(req: HttpRequest, name: web::Path<String>) -> String {
    //println!("REQ: {:?}", req);
    let env_name = env::var("DATABASE_URL").ok();
    //println!(env_name);
    //format!("Hello sync: {:?} {:?}!\r\n", name, env_name.unwrap()) // print quotes
    format!("Hello sync: {:?} {:}!\r\n", name, env_name.unwrap())
}

#[get("/db2")]
pub fn db2(req: HttpRequest) -> String {
    //println!("REQ: {:?}", req);
    let env_name = env::var("DATABASE_URL").ok();
    //println!(env_name);
    //format!("Hello sync: {:?} {:?}!\r\n", name, env_name.unwrap()) // print quotes
    format!("Hello sync: {:}!\r\n", env_name.unwrap())
}

pub fn index_async(
    _req: HttpRequest,
    name: web::Path<String>,
) -> impl IntoFuture<Item = String, Error = Error> {
    //impl IntoFuture<Item = &'static str, Error = Error> {
    //println!("REQ: {:?}", req);
    Ok(format!("Hello async: {}!\r\n", name))
}

#[get("/")]
pub fn no_params() -> &'static str {
    "Hello world!\r\n"
}
