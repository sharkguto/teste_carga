#[derive(Queryable)]
#[table_name = "tasks"]
pub struct Company {
    pub id: i32,
    pub name: String,
    pub age: i32,
    pub address: String,
    pub salary: f32
}