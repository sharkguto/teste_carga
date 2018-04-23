using System.Collections.Generic;
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
        public async Task<IEnumerable<Company>> ListarTodos () {
            using (NpgsqlConnection conexao = new NpgsqlConnection (
                _configuracoes.GetConnectionString ("BaseIndicadores"))) {
                return await conexao.GetAllAsync<Company> ();
            }
        }
    }
}