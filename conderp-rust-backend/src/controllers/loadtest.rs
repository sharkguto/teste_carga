use futures::IntoFuture;
use diesel::pg::PgConnection;
//use serde_derive::{Deserialize, Serialize};
use crate::interfaces::db::{PgPool};
//use diesel::connection::Connection;
//use diesel::dsl::sql;
use actix_web::{web, get, Error, HttpRequest,HttpResponse};
use diesel::query_dsl::RunQueryDsl;
use diesel;

// #[derive(Debug, Serialize, Deserialize)]
// struct Company {
//     id: i32,
//     salary: f32,
//     address: String,
//     age: i8,
//     name: String
// }


// use diesel::sql_types::{Text,Integer,Float};

//use diesel::sql_query;


// #[get("/db2")]


pub fn db2(pool: web::Data<PgPool>) -> HttpResponse {

    let conn: &PgConnection = &pool.get().unwrap();

    let result = diesel::sql_query("SELECT id,salary,address,age,name FROM test.company").get_results(&conn).unwrap();

    //let result = sql::<(Integer,Float,Text,Integer,Text)>("SELECT id,salary,address,age,name FROM test.company").get_results(conn).expect("falhou");

    //let result = conn.execute("SELECT id,salary,address,age,name FROM test.company");

    //let mut result = sql_query("SELECT id,salary,address,age,name FROM test.company").load(&conn);

    //let comp = Company{id:1,salary:2.222,address:"zxczxc",age:22,name:"asdasdasdasdasdsd"};

    HttpResponse::Ok().json(result)
    
}





// #[get("/db2")]
// pub fn db2(pool: web::Data<PgPool>) -> HttpResponse {

//     let conn: &PgConnection = &pool.get().unwrap();
    
//     //let mut result = &conn.execute("SELECT salary,address,age,id,name FROM test.company").unwrap();
//     //let result = conn.execute("SELECT id,salary,address,age,name FROM test.company").unwrap();
//     let mut result = &conn.execute("SELECT id,salary,address,age,name FROM test.company").unwrap();


//     // .and_then(|row|
//     //     Company {
//     //         id: row::get(0),
//     //         salary: row.get(1),
//     //         address: row.get(2),
//     //         age: row.get(3),
//     //         name: row.get(4),
//     //     }
//     // );
    
//     //HttpResponse::Ok().json(result[0])
//     HttpResponse::Ok().json(result)
    
// }

pub fn db2_async(_req: HttpRequest) -> impl IntoFuture<Item = String, Error = Error> {
    Ok(format!("ok"))
}



// let mut result = &conn.execute("SELECT id,salary,address,age,name FROM test.company").unwrap().map(NO_PARAMS, |row|
// Ok(
//     Company {
//         id: row::get(0),
//         salary: row.get(1),
//         address: row.get(2),
//         age: row.get(3),
//         name: row.get(4),
//     }
// ));