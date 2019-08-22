// mod util;

mod controllers;

use actix_web::{middleware, web, App, HttpResponse, HttpServer};
use controllers::load::{index, index_async, no_params, db2};
use dotenv::dotenv;

fn main() -> std::io::Result<()> {
    dotenv().ok();
    std::env::set_var("RUST_LOG", "actix_server=info,actix_web=error");

    HttpServer::new(move || {
        App::new()
            .wrap(middleware::DefaultHeaders::new().header("X-Version", "0.2"))
            .wrap(middleware::Compress::default())
            .wrap(middleware::Logger::default())
            .service(index)
            .service(db2)
            .service(no_params)
            .service(
                web::resource("/v2/hello/{name}")
                    .wrap(middleware::DefaultHeaders::new().header("X-Version-R2", "0.3"))
                    .default_service(web::route().to(|| HttpResponse::MethodNotAllowed()))
                    .route(web::get().to_async(index_async)),
            )
            .service(web::resource("/").to(|| "Test\r\n"))
    })
    .bind(format!(
        "0.0.0.0:{:}",
        std::env::var("SERVER_PORT").ok().unwrap()
    ))?
    .workers(4)
    .run()
}
