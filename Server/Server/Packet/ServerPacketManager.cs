using System;
using ServerCore;
using System.Collections.Generic;

class PacketManager
{
    #region Singleton
    static PacketManager instance;
    public static PacketManager Instance { get{ return instance; } }
    #endregion

    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>> onRecv = 
        new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>>();
    Dictionary<ushort, Action<PacketSession, IPacket>> handler =
        new Dictionary<ushort, Action<PacketSession, IPacket>>();

    PacketManager()
    {
        Register();
    }

    public void Register()
    {
        onRecv.Add((ushort)PacketID.C_Chat, MakePacket<C_Chat>);
        handler.Add((ushort)PacketID.C_Chat, PacketHandler.C_ChatHandler);

    }

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += sizeof(ushort);

        ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += sizeof(ushort);

        Action<PacketSession, ArraySegment<byte>> action = null;
        if(onRecv.TryGetValue(id, out action))
            action(session, buffer);
    }

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T : IPacket, new()
    {
        T pkt = new T();
        pkt.Read(buffer);

        Action<PacketSession, IPacket> action = null;
        if (handler.TryGetValue(pkt.Protocol, out action))
            action.Invoke(session, pkt);
    }
}