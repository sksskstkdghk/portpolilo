using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    internal class PacketFormat
    {
        // {0} 패킷 등록
        public static string managerFormat =
@"using System;
using ServerCore;
using System.Collections.Generic;

class PacketManager
{{
    #region Singleton
    static PacketManager instance;
    public static PacketManager Instance
    {{
        get
        {{
            if(instance == null)
                instance = new PacketManager();
            return instance;
        }}
    }}
    #endregion

    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>> onRecv = 
        new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>>();
    Dictionary<ushort, Action<PacketSession, IPacket>> handler =
        new Dictionary<ushort, Action<PacketSession, IPacket>>();

    public void Register()
    {{
{0}
    }}

    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {{
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += sizeof(ushort);

        ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += sizeof(ushort);

        Action<PacketSession, ArraySegment<byte>> action = null;
        if(onRecv.TryGetValue(id, out action))
            action(session, buffer);
    }}

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T : IPacket, new()
    {{
        T pkt = new T();
        pkt.Read(buffer);

        Action<PacketSession, IPacket> action = null;
        if (handler.TryGetValue(pkt.Protocol, out action))
            action.Invoke(session, pkt);
    }}
}}";
        // {0} 패킷 이름
        public static string managerRegisterFormat =
@"        onRecv.Add((ushort)PacketID.{0}, MakePacket<{0}>);
        handler.Add((ushort)PacketID.{0}, PacketHandler.{0}Handler);";

        // {0} 패킷 이름/ 번호 목록
        // {1} 패킷 목록
        public static string fileFormat =
@"using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;

public enum PacketID
{{
    {0}
}}

interface IPacket
{{
	ushort Protocol {{ get; }}
	void Read(ArraySegment<Byte> buffer);
	ArraySegment<byte> Write();
}}

{1}";
        // {0} 패킷 이름
        // {1} 패킷 번호
        public static string packetEnumFormat = @"{0} = {1},";

        // {0} 패킷 이름
        // {1} 멤버 변수
        // {2} 멤버 변수 Read
        // {3} 멤버 변수 write
        public static string packetFormat =
@"
class {0} : IPacket
{{
    {1}

    public ushort Protocol {{ get {{ return (ushort)PacketID.{0}; }} }}

    public ArraySegment<byte> Write()
    {{
        ArraySegment<byte> segement = SendBufferHelper.Open(4096);

        ushort count = sizeof(ushort);
        bool success = true;

        Span<byte> s = new Span<byte>(segement.Array, segement.Offset, segement.Count);

        success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)PacketID.{0});
        count += sizeof(ushort);

        {3}

        success &= BitConverter.TryWriteBytes(s, count);
        if (!success)
            return null;

        return SendBufferHelper.Close(count);
    }}

    public void Read(ArraySegment<byte> buffer)
    {{
        ushort count = 0;

        ReadOnlySpan<byte> s = buffer.Array;

        //ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += sizeof(ushort);

        //ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += sizeof(ushort);

        {2}
    }}
}}";
        // {0} 변수 형식
        // {1} 변수 이름
        public static string memberFormat = @"public {0} {1};";

        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        // {2} 멤버 변수
        // {3} 멤버 변수 Read
        // {4} 멤버 변수 write
        public static string memberListFormat =
@"
public class {0}
{{
    {2}

    public bool Write(Span<byte> s, ref ushort count)
    {{
        bool success = true;

        {4}

        return success;
    }}

    public void Read(ReadOnlySpan<byte> s, ref ushort count)
    {{
        {3}
    }}
}}

public List<{0}> {1}s = new List<{0}>();";
        // {0} 변수 이름
        // {1} to~ 변수 형식
        // {2} 변수 형식
        public static string readFormat =
@"this.{0} = BitConverter.{1}(s.Slice(count, s.Length - count));
count += sizeof({2});";
        // {0} 변수 이름
        // {1} 변수 형식
        public static string readByteFormat =
@"this.{0} = ({1})buffer.Array[buffer.Offset + count];
count += sizeof({1});
";
        // {0} 변수 이름
        public static string readStringFormat =
@"ushort {0}Len = BitConverter.ToUInt16(s.Slice(count, s.Length - count));
count += sizeof(ushort);
this.{0} = Encoding.Unicode.GetString(s.Slice(count, {0}Len));
count += {0}Len;";
        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        public static string readListFormat =
@"this.{1}s.Clear();
ushort {1}Len = BitConverter.ToUInt16(s.Slice(count, s.Length - count));
count += sizeof(ushort);
for(int i =0; i< {1}Len; i++)
{{
    {0} {1} = new {0}();
    {1}.Read(s, ref count);
    this.{1}s.Add({1});
}}";
        // {0} 변수 이름
        // {1} 변수 형식
        public static string writeForamt =
@"success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), this.{0});
count += sizeof({1});";
        // {0} 변수 이름
        // {1} 변수 형식
        public static string writeByteFormat =
@"segement.Array[segement.Offset + count] = (byte)this.{0};
count += sizeof({1});";

        // {0} 변수 이름
        public static string writeStringFormat =
@"ushort {0}Len = (ushort)Encoding.Unicode.GetBytes(this.{0}, 0, this.{0}.Length, segement.Array, segement.Offset + count + sizeof(ushort));
success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), {0}Len);
count += sizeof(ushort);
count += {0}Len;";
        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        public static string writeListFormat =
@"success &= BitConverter.TryWriteBytes(s.Slice(count, s.Length - count), (ushort)this.{1}s.Count);
count += sizeof(ushort);
foreach({0} {1} in this.{1}s)
    success &= {1}.Write(s, ref count);";
    }
}
