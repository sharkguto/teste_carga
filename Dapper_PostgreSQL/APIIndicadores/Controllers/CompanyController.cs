using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace APIIndicadores.Controllers {
    //[Route ("api/[controller]")]
    [Route ("/db2")]
    public class CompanyController : Controller {
        [HttpGet]
        public IActionResult Get (
            [FromServices] CompanyDAO dao) {
            var result = dao.ListarTodosSync ();
            return Ok (result);
        }
        // public async Task<IActionResult> Get (
        //     [FromServices] CompanyDAO dao) {
        //     var result = await dao.ListarTodos ();
        //     return Ok (result);
        // }
    }
}