module main

import (
	net
	http 
	vweb
	os
	pg
)

const (
	port = 8080
	db_name = 'postgres'
	db_user = 'gustavo'
    db_pass = 'test'
)

struct App {
pub mut:
	vweb vweb.Context // TODO embed 
	db pg.DB
	cur_user User 
}

fn main() {
	println('Running vorum on http://0.0.0.0:$port')
	vweb.run<App>(port)
}


pub fn (app mut App) init() {
	app.db = pg.connect(db_name, db_user,db_pass) 
	
}