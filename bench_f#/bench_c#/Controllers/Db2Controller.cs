using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace bench_c_.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class Db2Controller : ControllerBase
    {
        [HttpGet]
        public String Get()
        {
            return "asdasdsad";
        }
    }
}
