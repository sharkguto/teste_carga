
extern crate diesel;
extern crate r2d2;

use diesel::r2d2::{ConnectionManager, Pool};
//use diesel::r2d2::{Error};
use diesel::result::Error;
use diesel::pg::PgConnection;
use diesel::prelude::*;


pub struct DbExecutor(pub Pool<ConnectionManager<PgConnection>>);

#[derive(Queryable)]
#[table_name = "tasks"]
pub struct Company {
    pub id: i32,
    pub name: String,
    pub age: i32,
    pub address: String,
    pub salary: f32
}