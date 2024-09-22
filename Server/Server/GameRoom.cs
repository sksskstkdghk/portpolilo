using ServerCore;
using System;
using System.Collections.Generic;
using System.Text;

namespace Server
{
    class GameRoom : IJobQueue
    {
        List<ClientSession> sessions = new List<ClientSession>();
        JobQueue jobQueue = new JobQueue();

        List<ArraySegment<byte>> pendingList = new List<ArraySegment<byte>>();

        public void Enter(ClientSession session)
        {
            sessions.Add(session);
            session.Room = this;

            // 신입생한테 모든 플레이어 목록 전송
            S_PlayerList players = new S_PlayerList();
            foreach(ClientSession temp in sessions)
            {
                players.players.Add(new S_PlayerList.Player()
                { 
                    isSelf = (temp == session),
                    playerId = temp.SessionId,
                    posX = temp.PosX,
                    posY = temp.PosY,
                    posZ = temp.PosZ,
                });
            }
            session.Send(players.Write());

            // 신입생 입장을 모두에게 알림
            S_BroadcastEnterGame enter = new S_BroadcastEnterGame();
            enter.playerId = session.SessionId;
            enter.posX = 0;
            enter.posY = 0;
            enter.posZ = 0;
            Broadcast(enter.Write());
        }

        public void Flush()
        {
            foreach (ClientSession s in sessions)
            {
                s.Send(pendingList);
            }

            //Console.WriteLine($"Flushed {pendingList.Count} items");
            pendingList.Clear();
        }

        // 해당 룸 또는 서버 전체가 영향 받는 데이터 보낼 때
        public void Broadcast(ArraySegment<byte> segement)
        {
            pendingList.Add(segement);
        }

        public void Leave(ClientSession session)
        {
            // 플레이어 제거
            sessions.Remove(session);

            // 모두에게 알림
            S_BroadcastLeaveGame leave = new S_BroadcastLeaveGame();
            leave.playerId = session.SessionId;
            Broadcast(leave.Write());
        }

        // 유저 개인이 영향받는 데이터를 보낼 때
        public void Push(Action job)
        {
            jobQueue.Push(job);
        }

        #region server -> client
        public void Move(ClientSession session, C_Move packet)
        {
            // 좌표 변경
            session.PosX = packet.posX; 
            session.PosY = packet.posY; 
            session.PosZ = packet.posZ;

            // 모두에게 알림
            S_BroadcastMove move = new S_BroadcastMove();
            move.playerId = session.SessionId;
            move.posX = session.PosX; 
            move.posY = session.PosY; 
            move.posZ = session.PosZ;
            Broadcast(move.Write());
        }
        #endregion
    }
}
