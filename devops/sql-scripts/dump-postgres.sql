--
-- PostgreSQL database dump
--

-- Dumped from database version 11.4 (Debian 11.4-1.pgdg90+1)
-- Dumped by pg_dump version 11.4 (Ubuntu 11.4-1.pgdg18.04+1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: test; Type: SCHEMA; Schema: -; Owner: gustavo
--

CREATE SCHEMA test;


ALTER SCHEMA test OWNER TO gustavo;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: company; Type: TABLE; Schema: test; Owner: gustavo
--

CREATE TABLE test.company (
    id integer NOT NULL,
    name text NOT NULL,
    age integer NOT NULL,
    address character(50),
    salary real
);


ALTER TABLE test.company OWNER TO gustavo;

--
-- Data for Name: company; Type: TABLE DATA; Schema: test; Owner: gustavo
--

INSERT INTO test.company VALUES (10, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (9, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (8, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (7, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (6, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (5, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (4, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (3, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (2, 'b', 2, 'n', 2.0999999);
INSERT INTO test.company VALUES (1, 'b', 2, 'n', 2.0999999);


--
-- Name: company company_pkey; Type: CONSTRAINT; Schema: test; Owner: gustavo
--

ALTER TABLE ONLY test.company
    ADD CONSTRAINT company_pkey PRIMARY KEY (id);


--
-- PostgreSQL database dump complete
--

