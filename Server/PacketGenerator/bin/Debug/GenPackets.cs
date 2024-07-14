using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;

public enum PacketID
{
    PlayerInfoReq = 1,
	Test = 2,
	
}

interface IPacket
{
	ushort Protocol { get; }
	void Read(ArraySegment<Byte> buffer);
	ArraySegment<byte> Write();
}


class PlayerInfoReq : IPacket
{
    public byte testByte;
	public long playerId;
	public string name;
	
	public class Skill
	{
	    public int id;
		public short level;
		public float duration;
		
		public class Attribute
		{
		    public int att;
		
		    public bool Write(Span<byte> s, ref ushort count)
		    {
		        bool success = true;
		
		        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.att);
				count += sizeof(int);
				
		
		        return success;
		    }
		
		    public void Read(ReadOnlySpan<byte> s, ref ushort count)
		    {
		        this.att = BitConverter.ToInt32(s.Slice(count, s.Length - count));
				count += sizeof(int);
				
		    }
		}
		
		public List<Attribute> attributes = new List<Attribute>();
		
	
	    public bool Write(Span<byte> s, ref ushort count)
	    {
	        bool success = true;
	
	        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.id);
			count += sizeof(int);
			
			success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.level);
			count += sizeof(short);
			
			success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.duration);
			count += sizeof(float);
			
			success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)this.attributes.Count);
			count += sizeof(ushort);
			foreach(Attribute attribute in this.attributes)
			    success &= attribute.Write(s, ref count);
			
	
	        return success;
	    }
	
	    public void Read(ReadOnlySpan<byte> s, ref ushort count)
	    {
	        this.id = BitConverter.ToInt32(s.Slice(count, s.Length - count));
			count += sizeof(int);
			
			this.level = BitConverter.ToInt16(s.Slice(count, s.Length - count));
			count += sizeof(short);
			
			this.duration = BitConverter.ToSingle(s.Slice(count, s.Length - count));
			count += sizeof(float);
			
			this.attributes.Clear();
			ushort attributeLen = BitConverter.ToUInt16(s.Slice(count, s.Length - count));
			count += sizeof(ushort);
			for(int i =0; i< attributeLen; i++)
			{
			    Attribute attribute = new Attribute();
			    attribute.Read(s, ref count);
			    this.attributes.Add(attribute);
			}
			
	    }
	}
	
	public List<Skill> skills = new List<Skill>();
	

    public ushort Protocol { get { return (ushort)PacketID.PlayerInfoReq; } }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> segement = SendBufferHelper.Open(4096);

        ushort count = sizeof(ushort);
        bool success = true;

        Span<byte> s = new Span<byte>(segement.Array, segement.Offset, segement.Count);

        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)PacketID.PlayerInfoReq);
        count += sizeof(ushort);

        segement.Array[segement.Offset + count] = (byte)this.testByte;
		count += sizeof(byte);
		
		success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.playerId);
		count += sizeof(long);
		
		ushort nameLen = (ushort)Encoding.Unicode.GetBytes(this.name, 0, this.name.Length, segement.Array, segement.Offset + count + sizeof(ushort));
		success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), nameLen);
		count += sizeof(ushort);
		count += nameLen;
		
		success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)this.skills.Count);
		count += sizeof(ushort);
		foreach(Skill skill in this.skills)
		    success &= skill.Write(s, ref count);
		

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

        this.testByte = (byte)buffer.Array[buffer.Offset + count];
		count += sizeof(byte);
		
		this.playerId = BitConverter.ToInt64(s.Slice(count, s.Length - count));
		count += sizeof(long);
		
		ushort nameLen = BitConverter.ToUInt16(s.Slice(count, s.Length - count));
		count += sizeof(ushort);
		this.name = Encoding.Unicode.GetString(s.Slice(count, nameLen));
		count += nameLen;
		
		this.skills.Clear();
		ushort skillLen = BitConverter.ToUInt16(s.Slice(count, s.Length - count));
		count += sizeof(ushort);
		for(int i =0; i< skillLen; i++)
		{
		    Skill skill = new Skill();
		    skill.Read(s, ref count);
		    this.skills.Add(skill);
		}
		
    }
}

class Test : IPacket
{
    public int testInt;

    public ushort Protocol { get { return (ushort)PacketID.Test; } }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> segement = SendBufferHelper.Open(4096);

        ushort count = sizeof(ushort);
        bool success = true;

        Span<byte> s = new Span<byte>(segement.Array, segement.Offset, segement.Count);

        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)PacketID.Test);
        count += sizeof(ushort);

        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.testInt);
		count += sizeof(int);
		

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

        this.testInt = BitConverter.ToInt32(s.Slice(count, s.Length - count));
		count += sizeof(int);
		
    }
}

