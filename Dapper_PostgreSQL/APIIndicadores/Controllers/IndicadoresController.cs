using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;

namespace APIIndicadores.Controllers
{
    [Route("api/[controller]")]
    public class IndicadoresController : Controller
    {
        [HttpGet]
        public IEnumerable<Indicador> Get(
            [FromServices]IndicadoresDAO indicadoresDAO)
        {
            return indicadoresDAO.ListarTodos();
        }
    }
}