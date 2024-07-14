using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace ServerCore
{
    public abstract class PacketSession : Session
    {
        public static readonly int headerSize = 2;

        // sealed : 현재 클래스 하위 클래스에서 해당 함수를 다시 override할 수 없게 봉인 한다는 의미
        // [size(2)][packetID(2)][...][size(2)][packetID(2)][...]...
        public sealed override int OnRecv(ArraySegment<byte> buffer)
        {
            int processLen = 0;

            while(true)
            {
                // 최소한 헤더는 파싱 가능한지 확인
                if (buffer.Count < headerSize)
                    break;

                // 패킷이 전부 도착했는지 확인
                ushort dataSize = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
                if (buffer.Count < dataSize)
                    break;

                // 위 다 통과했다면 패킷 조립 가능
                OnRecvPacket(new ArraySegment<byte> (buffer.Array, buffer.Offset, dataSize));

                processLen += dataSize;
                buffer = new ArraySegment<byte>(buffer.Array, buffer.Offset + dataSize, buffer.Count - dataSize);
            }

            return processLen;
        }

        public abstract void OnRecvPacket(ArraySegment<byte> buffer);
    }

    public abstract class Session
    {
        Socket socket;
        SocketAsyncEventArgs recvArgs = new SocketAsyncEventArgs();
        SocketAsyncEventArgs sendArgs = new SocketAsyncEventArgs();

        RecvBuffer recvBuffer = new RecvBuffer(1024);

        int disconnected = 0;
        Queue<ArraySegment<byte>> sendQueue = new Queue<ArraySegment<byte>>();
        List<ArraySegment<byte>> pendingList = new List<ArraySegment<byte>>();

        object _lock = new object();

        public abstract void OnConnected(EndPoint endPoint);
        public abstract void OnDisconnected(EndPoint endPoint);
        public abstract int OnRecv(ArraySegment<byte> buffer);
        public abstract void OnSend(int numOfBytes);

        void Clear()
        {
            lock (_lock)
            {
                sendQueue.Clear();
                pendingList.Clear();
            }
        }

        public void Start(Socket clientSocket)
        {
            socket = clientSocket;
            
            recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnRecvCompleted);
            sendArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnSendCompleted);

            RegisterRecv();
        }

        #region 네트워크 통신
        private void RegisterRecv()
        {
            if (disconnected == 1)
                return;

            try
            {
                recvBuffer.Clean();
                ArraySegment<byte> segment = recvBuffer.WriteSegment;
                recvArgs.SetBuffer(segment.Array, segment.Offset, segment.Count);

                bool pending = socket.ReceiveAsync(recvArgs);

                if (!pending)
                    OnRecvCompleted(null, recvArgs);
            }
            catch (Exception e)
            {
                Console.WriteLine($"RegisterRecv Failed {e}");
            }
        }

        private void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
        {
            if(args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
            {
                try
                {
                    // write 커서 이동
                    if(recvBuffer.OnWrite(args.BytesTransferred) == false)
                    {
                        Discconect();
                        return;
                    }

                    // 컨텐츠 쪽으로 데이터를 넘겨주고 얼마나 처리했는지 받는다.
                    int procession = OnRecv(recvBuffer.ReadSegment);
                    if(procession < 0 || recvBuffer.DataSize < procession)
                    {
                        Discconect();
                        return;
                    }

                    // Read 커서 이동
                    if(recvBuffer.OnRead(procession) == false)
                    {
                        Discconect();
                        return;
                    }

                    RegisterRecv();
                }
                catch (Exception e)
                { 
                    Console.WriteLine($"OnRecvCompleted Failed {e.ToString()}");
                }
            }
            else
            {
                Discconect();
            }
        }

        private void RegisterSend()
        {
            if (disconnected == 1)
                return;

            while (sendQueue.Count > 0)
            {
                ArraySegment<byte> buff = sendQueue.Dequeue();
                //sendArgs.SetBuffer(buff, 0, buff.Length);

                pendingList.Add(buff);
            }

            try
            {
                sendArgs.BufferList = pendingList;

                bool pending = socket.SendAsync(sendArgs);
                if (!pending)
                    OnSendCompleted(null, sendArgs);
            }
            catch (Exception e)
            {
                Console.WriteLine($"RegisterSend Failed {e}");
            }
        }

        private void OnSendCompleted(object sender, SocketAsyncEventArgs args)
        {
            lock (_lock)
            {
                if (args.BytesTransferred > 0 && args.SocketError == SocketError.Success)
                {
                    try
                    {
                        sendArgs.BufferList = null;
                        pendingList.Clear();

                        OnSend(sendArgs.BytesTransferred);

                        if(sendQueue.Count > 0)
                            RegisterSend();
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine($"OnSendCompleted Failed {e.ToString()}");
                    }
                }
                else
                {
                    Discconect();
                }
            }
        }

        // 보내기(send)
        public void Send(ArraySegment<byte> sendBuff)
        {
            lock (_lock) 
            {
                sendQueue.Enqueue(sendBuff);

                if (pendingList.Count <= 0)
                    RegisterSend();
            }
        }

        // 퇴장
        public void Discconect()
        {
            if (Interlocked.Exchange(ref disconnected, 1) == 1)
                return;

            OnDisconnected(socket.RemoteEndPoint);
            socket.Shutdown(SocketShutdown.Both);
            socket.Close();

            Clear();
        }
        #endregion
    }
}
