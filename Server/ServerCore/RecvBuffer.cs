using System;
using System.Collections.Generic;
using System.Text;

namespace ServerCore
{
    public class RecvBuffer
    {
        ArraySegment<byte> buffer;
        int readPos;
        int writePos;

        public RecvBuffer(int bufferSize)
        {
            buffer = new ArraySegment<byte>(new byte[bufferSize], 0, bufferSize);
        }

        public int DataSize { get { return writePos - readPos; } }  // 현재 받은 데이터 크기
        public int FreeSize { get { return buffer.Count - writePos; } } // 남아 있는 버퍼 크기

        public ArraySegment<byte> ReadSegment
        {
            get { return new ArraySegment<byte>(buffer.Array, buffer.Offset + readPos, DataSize); }
        }

        public ArraySegment<byte> WriteSegment
        {
            get { return new ArraySegment<byte>(buffer.Array, buffer.Offset + writePos, FreeSize); }
        }

        public void Clean()
        {
            int dataSize = DataSize;

            if(dataSize <= 0)   // 데이터 전부 처리 완료
            {
                readPos = writePos = 0;
                
            }
            else // 아직 데이터 처리가 다 안됨
            {
                Array.Copy(buffer.Array, buffer.Offset + readPos, buffer.Array, buffer.Offset, dataSize);
                readPos = 0;
                writePos = dataSize;
            }
        }

        public bool OnRead(int numOfBytes)
        {
            if (numOfBytes > DataSize)
                return false;

            readPos += numOfBytes;

            return true;
        }

        public bool OnWrite(int numOfBytes) 
        {
            if (numOfBytes > FreeSize)
                return false;

            writePos += numOfBytes;

            return true;
        }
    }
}
