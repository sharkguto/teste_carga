using System.Collections.Generic;
using Npgsql;
using Microsoft.Extensions.Configuration;
using Dapper.Contrib.Extensions;

namespace APIIndicadores
{
    public class IndicadoresDAO
    {
        private IConfiguration _configuracoes;

        public IndicadoresDAO(IConfiguration config)
        {
            _configuracoes = config;
        }

        public IEnumerable<Indicador> ListarTodos()
        {
            using (NpgsqlConnection conexao = new NpgsqlConnection(
                _configuracoes.GetConnectionString("BaseIndicadores")))
            {
                return conexao.GetAll<Indicador>();
            }
        }
    }
}