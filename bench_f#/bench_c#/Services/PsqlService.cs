using Npgsql;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace bench_c_.Services
{
    public class PsqlService
    {
       public class PoolController
        {
            public int ConnectionID { get; set; }
            public NpgsqlConnection Connection { get; set; }
            public bool InUse { get; set; } = false;
        }

        private static List<PoolController> pConnections = new List<PoolController>();
        public static void StartPool(string sConnectionString, int iMaxPool)
        {
            sConnectionString = string.Format(sConnectionString, iMaxPool);
            while (iMaxPool > 0)
            {
                NpgsqlConnection pConnection = new NpgsqlConnection(sConnectionString);
                pConnection.Open();

                pConnections.Add(new PoolController() { Connection = pConnection, InUse = false, ConnectionID = iMaxPool });
                iMaxPool--;
            }
        }

        public static PoolController GetConnection()
        {
            PoolController pConnectionController = null;

            while (pConnectionController == null)
            {
                if (pConnections.Where(x => !x.InUse).Count() > 0)
                {
                    try
                    {
                        pConnectionController = pConnections.First(x => !x.InUse);
                        pConnectionController.InUse = true;

                        return pConnectionController;
                    }
                    catch
                    {
                        //DO NOTHING
                    }
                }

                Thread.Sleep(1);
            }

            return pConnectionController;
        }

        public static void ReleaseConnection(PoolController poolController)
        {
            poolController.InUse = false;
        }
    }
}