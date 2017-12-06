create schema test;


CREATE TABLE test.COMPANY(
   ID INT PRIMARY KEY     NOT NULL,
   NAME           TEXT    NOT NULL,
   AGE            INT     NOT NULL,
   ADDRESS        CHAR(50),
   SALARY         REAL
);


select * from test.company;

insert into test.company values(10,'b',2,'n',2.1);
insert into test.company values(9,'b',2,'n',2.1);
insert into test.company values(8,'b',2,'n',2.1);
insert into test.company values(7,'b',2,'n',2.1);
insert into test.company values(6,'b',2,'n',2.1);
insert into test.company values(5,'b',2,'n',2.1);
insert into test.company values(4,'b',2,'n',2.1);
insert into test.company values(3,'b',2,'n',2.1);
insert into test.company values(2,'b',2,'n',2.1);
insert into test.company values(1,'b',2,'n',2.1);


select count(*) from pg_catalog.pg_stat_activity;

select * from pg_catalog.pg_stat_activity;
