// use std::ops::Deref;
use diesel::pg::PgConnection;
use diesel::r2d2::{ConnectionManager, Pool};
//use diesel::r2d2::{ConnectionManager, Pool, PooledConnection};
use dotenv;

pub type PgPool = Pool<ConnectionManager<PgConnection>>;

// pub struct DbConn(pub PooledConnection<ConnectionManager<PgConnection>>);

// impl Deref for DbConn {
//     type Target = PgConnection;

//     fn deref(&self) -> &Self::Target {
//         &self.0
//     }
// }



pub fn init_pool() -> PgPool {
    dotenv::dotenv().ok();
    let connspec = std::env::var("DATABASE_URL").expect("DATABASE_URL");
    let manager = ConnectionManager::<PgConnection>::new(connspec);
    Pool::new(manager).expect("db pool")
}

