package com.teste_carga.bench_java;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

//import org.postgresql.ds.PGPoolingDataSource;
import org.postgresql.*;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
/**
 * Bench
 */
public class Bench {
    @RequestMapping("/db2")
    public Model2 defBench() throws Exception {

        // {
        //     'host': '127.0.0.1',
        //     'user': 'gustavo',
        //     'password': 'test',
        //     'port': '5432',
        //     'database': 'test'
        // }

        String url = "jdbc:postgresql://127.0.0.1:5432/test";
        //Properties props = new Properties();
        Statement stmt = null;
        ResultSet rs = null;
        String query = "SELECT salary,address,age,id,name FROM test.company";
        Model2 mm = null;

        PGPoolingDataSource connectionPool = new PGPoolingDataSource();
        connectionPool.setServerName("127.0.0.1");
        connectionPool.setPortNumber(5432);
        connectionPool.setDatabaseName("test");
        connectionPool.setUser("gustavo");
        connectionPool.setPassword("test");
        connectionPool.setMaxConnections(80);

        Connection conn = connectionPool.getConnection();

        stmt = conn.createStatement();
        rs = stmt.executeQuery(query);
        List<Model> posts = new ArrayList<Model>();

        while (rs.next()) {
            Float salary = rs.getFloat(1);
            String address = rs.getString(2);
            Integer age = rs.getInt(3);
            Integer id = rs.getInt(4);
            String name = rs.getString(5);
            Model m = new Model();
            m.setAddress(address);
            m.setAge(age);
            m.setId(id);
            m.setName(name);
            m.setSalary(salary);
            posts.add(m);

        }
        mm = new Model2(posts);

        if (conn != null) {
            try {
                conn.close();
            } catch (SQLException e) {
            }
        }

        return mm;
    }

}