using System;
using Dapper.Contrib.Extensions;

namespace APIIndicadores
{
    [Table("Indicadores")]
    public class Indicador
    {
        [ExplicitKey]
        public string Sigla { get; set; }
        public string NomeIndicador { get; set; }
        public DateTime UltimaAtualizacao { get; set; }
        public decimal Valor { get; set; }
    }
}