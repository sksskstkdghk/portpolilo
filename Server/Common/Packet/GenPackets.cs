using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;

public enum PacketID
{
    C_Chat = 1,
	S_Chat = 2,
	
}

interface IPacket
{
	ushort Protocol { get; }
	void Read(ArraySegment<Byte> buffer);
	ArraySegment<byte> Write();
}


class C_Chat : IPacket
{
    public string chat;

    public ushort Protocol { get { return (ushort)PacketID.C_Chat; } }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> segement = SendBufferHelper.Open(4096);

        ushort count = sizeof(ushort);
        bool success = true;

        Span<byte> s = new Span<byte>(segement.Array, segement.Offset, segement.Count);

        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)PacketID.C_Chat);
        count += sizeof(ushort);

        ushort chatLen = (ushort)Encoding.Unicode.GetBytes(this.chat, 0, this.chat.Length, segement.Array, segement.Offset + count + sizeof(ushort));
		success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), chatLen);
		count += sizeof(ushort);
		count += chatLen;

        success &= BitConverter.TryWriteBytes(s, count);
        if (!success)
            return null;

        return SendBufferHelper.Close(count);
    }

    public void Read(ArraySegment<byte> buffer)
    {
        ushort count = 0;

        ReadOnlySpan<byte> s = buffer.Array;

        //ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += sizeof(ushort);

        //ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += sizeof(ushort);

        ushort chatLen = BitConverter.ToUInt16(s.Slice(count, s.Length - count));
		count += sizeof(ushort);
		this.chat = Encoding.Unicode.GetString(s.Slice(count, chatLen));
		count += chatLen;
    }
}
class S_Chat : IPacket
{
    public int playerId;
	public string chat;

    public ushort Protocol { get { return (ushort)PacketID.S_Chat; } }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> segement = SendBufferHelper.Open(4096);

        ushort count = sizeof(ushort);
        bool success = true;

        Span<byte> s = new Span<byte>(segement.Array, segement.Offset, segement.Count);

        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)PacketID.S_Chat);
        count += sizeof(ushort);

        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.playerId);
		count += sizeof(int);
		ushort chatLen = (ushort)Encoding.Unicode.GetBytes(this.chat, 0, this.chat.Length, segement.Array, segement.Offset + count + sizeof(ushort));
		success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), chatLen);
		count += sizeof(ushort);
		count += chatLen;

        success &= BitConverter.TryWriteBytes(s, count);
        if (!success)
            return null;

        return SendBufferHelper.Close(count);
    }

    public void Read(ArraySegment<byte> buffer)
    {
        ushort count = 0;

        ReadOnlySpan<byte> s = buffer.Array;

        //ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += sizeof(ushort);

        //ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += sizeof(ushort);

        this.playerId = BitConverter.ToInt32(s.Slice(count, s.Length - count));
		count += sizeof(int);
		ushort chatLen = BitConverter.ToUInt16(s.Slice(count, s.Length - count));
		count += sizeof(ushort);
		this.chat = Encoding.Unicode.GetString(s.Slice(count, chatLen));
		count += chatLen;
    }
}