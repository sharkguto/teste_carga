using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using bench_c_.Services;
using Npgsql;
using bench_c_.Models;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace bench_c_.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class Db2Controller : ControllerBase
    {
        [HttpGet]
        public JsonResult Get()
        {
            var x = PsqlService.GetConnection();
            Npgsql.NpgsqlConnection conn = x.Connection;
            //conn.Open();
            NpgsqlCommand cmd = new NpgsqlCommand("SELECT salary,address,age,id,name FROM test.company; ",conn);
            NpgsqlDataReader dr = cmd.ExecuteReader();


            var smlist = new List<SalaryModel>();

            while (dr.Read()){
                SalaryModel sm = new SalaryModel
                {
                    id=(int) dr[3],
                    salary=decimal.Parse(dr[0].ToString()),
                    address=(string)dr[1],
                    age=(int)dr[2],
                    name=(string) dr[4]

                    
                };
                smlist.Add(sm);
                
            }
            dr.Close();
            
            PsqlService.ReleaseConnection(x);

            return new JsonResult(smlist);
            //return JsonSerializer.Serialize<List<SalaryModel>>(smlist);
        }
    }
}
