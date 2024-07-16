using System;
using System.Collections.Generic;
using System.Text;

namespace DummyClient
{
    class SessionManager
    {
        static SessionManager instance = new SessionManager();
        public static SessionManager Instance { get { return instance; } }

        List<ServerSession> sessions = new List<ServerSession>();
        object _lock = new object();

        public ServerSession Generate()
        {
            lock (_lock) 
            { 
                ServerSession session = new ServerSession();
                sessions.Add(session);
                return session;
            }
        }

        public void SendForEach()
        {
            lock (_lock)
            {
                foreach (ServerSession session in sessions)
                {
                    C_Chat chatPacket = new C_Chat();
                    chatPacket.chat = $"Hello Server!";
                    ArraySegment<byte> segement = chatPacket.Write();

                    session.Send(segement);
                }
            }
        }
    }
}
