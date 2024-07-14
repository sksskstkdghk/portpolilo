using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;

namespace DummyClient
{
    

    class ServerSession : Session
    {
        public override void OnConnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnConnected {endPoint}");

            C_PlayerInfoReq packet = new C_PlayerInfoReq() {playerId = 1001, name = "ABCDE" };
            var skill = new C_PlayerInfoReq.Skill() { id = 101, duration = 3.0f, level = 1 };
            skill.attributes.Add(new C_PlayerInfoReq.Skill.Attribute() { att = 77 });
            packet.skills.Add(skill);
            packet.skills.Add(new C_PlayerInfoReq.Skill() { id = 201, duration = 4.0f, level = 2 });
            packet.skills.Add(new C_PlayerInfoReq.Skill() { id = 301, duration = 5.0f, level = 3 });
            packet.skills.Add(new C_PlayerInfoReq.Skill() { id = 401, duration = 6.0f, level = 4 });

            ArraySegment<byte> sendBuff = packet.Write();
            if (sendBuff != null)
                Send(sendBuff);
        }

        public override void OnDisconnected(EndPoint endPoint)
        {
            Console.WriteLine($"OnDisconnected {endPoint}");
        }

        public override int OnRecv(ArraySegment<byte> buffer)
        {
            string recvData = Encoding.UTF8.GetString(buffer.Array, buffer.Offset, buffer.Count);
            Console.WriteLine($"[From Server] {recvData}");

            return 0;
        }

        public override void OnSend(int numOfBytes)
        {
            Console.WriteLine($"Send Transferred bytes: {numOfBytes}");
        }
    }
}
