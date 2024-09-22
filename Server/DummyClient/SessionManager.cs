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

        Random rand = new Random();

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
                    C_Move movePacket = new C_Move();
                    movePacket.posX = rand.Next(-50, 50);
                    movePacket.posY = 0;
                    movePacket.posZ = rand.Next(-50, 50);

                    session.Send(movePacket.Write());
                }
            }
        }
    }
}
