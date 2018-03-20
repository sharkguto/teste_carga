package com.teste_carga.bench_java;

import java.util.List;

/**
 * Model2
 */
public class Model2 {

    public List<Model> posts;

    public Model2() {
    }

    public Model2(List<Model> posts) {
        setPosts(posts);
    }

    /**
     * @return the posts
     */
    public List<Model> getPosts() {
        return posts;
    }

    /**
     * @param posts the posts to set
     */
    public void setPosts(List<Model> posts) {
        this.posts = posts;
    }
}
