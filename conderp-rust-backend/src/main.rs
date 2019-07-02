use actix_web::{HttpServer,middleware,App,web,HttpResponse};

mod controllers;

use controllers::hello::{ index_async, index, no_params};
use controllers::loadtest::{db2,db2_async };

fn main() -> std::io::Result<()> {
    std::env::set_var("RUST_LOG", "actix_server=info,actix_web=error");
    env_logger::init();

    HttpServer::new(|| {
        App::new()
            .wrap(middleware::DefaultHeaders::new().header("X-Version", "0.2"))
            .wrap(middleware::Compress::default())
            .wrap(middleware::Logger::default())
            .service(index)
            .service(no_params)
            .service(db2)
            .service(
                web::resource("/v2/hello/{name}")
                    .wrap(middleware::DefaultHeaders::new().header("X-Version-R2", "0.3"))
                    .default_service(web::route().to(|| HttpResponse::MethodNotAllowed()))
                    .route(web::get().to_async(index_async)),
            )
            .service(
                web::resource("/db2s")
                    .wrap(middleware::DefaultHeaders::new().header("X-Version-R2", "0.3"))
                    .default_service(web::route().to(|| HttpResponse::MethodNotAllowed()))
                    .route(web::get().to_async(db2_async)),
            )
            .service(web::resource("/").to(|| "Test\r\n"))
    })
    .bind("0.0.0.0:8080")?
    .workers(8)
    .run()
}

// extern crate actix_web;
// use actix_web::{server, App, HttpRequest, HttpResponse, Path, Responder};

// fn index(_req: &HttpRequest) -> &'static str {
//     "Hello world!"
// }

// fn index2(info: Path<(String)>) -> impl Responder {
//     format!("ola {}! bem vindo ao clune", info)
// }

// fn main() {
//     let server = server::new(|| {
//         vec![
//             App::new()
//                 .prefix("/v1")
//                 .resource("/hello/{name}", |r| r.with(index2)),
//             App::new().prefix("/app").resource("/", |r| r.f(index)),
//             App::new()
//                 .prefix("/app1")
//                 .resource("/", |r| r.f(|_r| HttpResponse::Ok())),
//             App::new()
//                 .prefix("/app2")
//                 .resource("/", |r| r.f(|_r| HttpResponse::Ok())),
//             App::new().resource("/", |r| r.f(|_r| HttpResponse::Ok())),
//         ]
//     });
//     server.bind("127.0.0.1:8080").unwrap().run();
//     // server::new(|| App::new().resource("/", |r| r.f(index)))
//     //     .bind("127.0.0.1:8088")
//     //     .unwrap()
//     //     .run();
// }
