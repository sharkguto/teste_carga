using System;
using System.Collections.Generic;
using System.Data;
using System.Reflection;
using System.Threading.Tasks;
using Dapper.Contrib.Extensions;
using Microsoft.Extensions.Configuration;
using Npgsql;

namespace APIIndicadores {
    public class CompanyDAO {
        private IConfiguration _configuracoes;

        public CompanyDAO (IConfiguration config) {
            _configuracoes = config;
        }

        public IEnumerable<Company> ListarTodosSync () {
            using (NpgsqlConnection conexao = new NpgsqlConnection (
                _configuracoes.GetConnectionString ("BaseIndicadores"))) {
                return conexao.GetAll<Company> ();
            }
        }
        
        public async Task<IEnumerable<Company>> ListarTodosTunning () {
            var companys = new List<Company> ();
            using (NpgsqlConnection conn = new NpgsqlConnection (_configuracoes.GetConnectionString ("BaseIndicadores"))) {
                conn.Open ();

                NpgsqlCommand cmd = new NpgsqlCommand ("SELECT * FROM test.company", conn);
                NpgsqlDataReader dr = cmd.ExecuteReader ();
                while (await dr.NextResultAsync ()) {
                    companys.Add (new Company {
                        ID = await dr.GetFieldValueAsync<int> (0),
                            NAME = await dr.GetFieldValueAsync<string> (1),
                            AGE = await dr.GetFieldValueAsync<int> (2),
                            ADDRESS = await dr.GetFieldValueAsync<string> (3),
                            SALARY = await dr.GetFieldValueAsync<decimal> (4)
                    });
                }
                dr.Close ();
            }
            return companys;
        }
        
        public async Task<IEnumerable<Company>> ListarTodos () {
            using (NpgsqlConnection conexao = new NpgsqlConnection (
                _configuracoes.GetConnectionString ("BaseIndicadores"))) {
                return await conexao.GetAllAsync<Company> ();
            }
        }
    }
}
