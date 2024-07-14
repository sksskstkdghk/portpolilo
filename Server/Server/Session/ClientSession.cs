using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;

namespace Server
{
    class ClientSession : PacketSession
    {
        public int SessionId { get; set; }
        public GameRoom Room { get; set; }

        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected {endPoint}");

            // 일단은 채팅 방 강제 연결
            Program.room.Enter(this);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            SessionManager.Instance.Remove(this);
            if(Room != null)
            {
                Room.Leave(this);
                Room = null;
            }

            Console.WriteLine($"OnDisconnected {endPoint}");
        }

        public override void OnRecvPacket(ArraySegment<byte> buffer)
        {
            PacketManager.Instance.OnRecvPacket(this, buffer);

        }

        public override void OnSend(int numOfBytes)
        {
            Console.WriteLine($"Send Transferred bytes: {numOfBytes}");
        }
    }
}
