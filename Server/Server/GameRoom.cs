using System;
using System.Collections.Generic;
using System.Text;

namespace Server
{
    class GameRoom
    {
        List<ClientSession> sessions = new List<ClientSession>();

        object _lock = new object();

        public void Enter(ClientSession session)
        {
            lock (_lock) 
            {
                sessions.Add(session);

                session.Room = this;
            }
        }

        public void Broadcast(ClientSession session, string chat)
        {
            S_Chat packet = new S_Chat();
            packet.playerId = session.SessionId;
            packet.chat = chat;

            ArraySegment<byte> segement = new ArraySegment<byte>();

            lock (_lock) 
            { 
                foreach(ClientSession s in sessions)
                {
                    s.Send(segement);
                }
            }
        }

        public void Leave(ClientSession session)
        {
            lock (_lock)
            {
                sessions.Remove(session);
            }
        }
    }
}
