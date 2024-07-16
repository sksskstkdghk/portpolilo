using System;
using System.Threading;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Text;
using ServerCore;

#region 네임스페이스
using SpinLock = LockClass.UserSpinLock;
using EventLock = LockClass.EventLock;
using ReaderWriterLock = LockClass.ReaderWriterLock;
using static System.Collections.Specialized.BitVector32;
#endregion

namespace Server
{
    class Program
    {
        static Listener listener = new Listener();
        public static GameRoom room = new GameRoom();
        static void Main(string[] args)
        {
            //PacketManager.Instance.Register();

            // DNS(Domain Name System)
            string host = Dns.GetHostName();
            IPHostEntry ipHost = Dns.GetHostEntry(host);
            IPAddress ipAddr = ipHost.AddressList[0];
            IPEndPoint endPoint = new IPEndPoint(ipAddr, 7777);

            listener.Init(endPoint, () => { return SessionManager.Instance.Generate(); });

            Console.WriteLine("Listening...");
            while (true)
            {
                
            }
        }
    }
}
