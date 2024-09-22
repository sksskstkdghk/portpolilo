using System;
using System.Collections.Generic;
using System.Text;

namespace Server
{
    class SessionManager
    {
        static SessionManager sessionManager = new SessionManager();
        public static SessionManager Instance { get { return sessionManager; } }

        int sessionId = 0;
        Dictionary<int, ClientSession> sessions = new Dictionary<int, ClientSession>();

        object _lock = new object();

        public ClientSession Generate()
        {
            lock (_lock)
            {
                int sessionId = ++this.sessionId;

                ClientSession session = new ClientSession();
                session.SessionId = sessionId;
                sessions.Add(sessionId, session);

                Console.WriteLine($"Connected : {sessionId}");

                return session;
            }
        }

        public ClientSession Find(int id)
        {
            lock (_lock)
            {
                ClientSession session = null;
                sessions.TryGetValue(id, out session);

                return session;
            }
        }

        public void Remove(ClientSession session)
        {
            lock (_lock)
            {
                sessions.Remove(session.SessionId);
            }
        }
    }
}
