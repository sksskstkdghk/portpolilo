using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace ServerCore
{
    public class SendBufferHelper
    {
        public static ThreadLocal<SendBuffer> CurrentBuffer = new ThreadLocal<SendBuffer>(() => { return null; });

        public static int ChunkSize { get; set; } = 4096 * 100;

        public static ArraySegment<byte> Open(int reserveSize)
        {
            if (CurrentBuffer.Value == null)
                CurrentBuffer.Value = new SendBuffer(ChunkSize);

            if(CurrentBuffer.Value.FreeSize < reserveSize)
                CurrentBuffer.Value = new SendBuffer(ChunkSize);

            return CurrentBuffer.Value.Open(reserveSize);
        }

        public static ArraySegment<byte> Close(int usedSize)
        {
            return CurrentBuffer.Value.Close(usedSize);
        }
    }

    public class SendBuffer
    {
        byte[] buffer;
        int usedSize = 0;

        public SendBuffer(int chunkSize)
        {
            buffer = new byte[chunkSize];
        }

        public int FreeSize { get { return buffer.Length - usedSize; } }

        /// <summary>
        /// 얼마나 사용할 지 모르니 임의 크기 만큼 사용한다고 일단 잡아 버퍼를 리턴하는 함수
        /// </summary>
        /// <param name="reserveSize">예상하는 최대 크기(어름짐작)</param>
        /// <returns></returns>
        public ArraySegment<byte> Open(int reserveSize)
        {
            if (reserveSize > FreeSize)
                return null;

            return new ArraySegment<byte>(buffer, usedSize, reserveSize);
        }

        /// <summary>
        /// 버퍼 사용 후 실제 사용한 크기만큼 잡아 버퍼를 리턴하는 함수
        /// </summary>
        /// <param name="usedSize">실제 사용한 크기</param>
        /// <returns></returns>
        public ArraySegment<byte> Close(int usedSize)
        {
            ArraySegment<byte> segment = new ArraySegment<byte>(buffer, this.usedSize, usedSize);
            this.usedSize += usedSize;
            return segment;
        }
    }
}
