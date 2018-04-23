using System;
using Dapper.Contrib.Extensions;

namespace APIIndicadores
{
    [Table("Test.Company")]
    public class Company
    {
        [Key]
        public Int32 ID { get; set; }
        public string NAME { get; set; }
        public Int32 AGE { get; set; }
        public string ADDRESS { get; set; }
        public decimal SALARY { get; set; }
    }
}