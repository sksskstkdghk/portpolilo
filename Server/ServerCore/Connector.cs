using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ServerCore
{
    // 서버 연결을 담당하는 클래스(연결 요청 보내기) - send
    public class Connector
    {
        Func<Session> sessionFactory;

        public void Connect(IPEndPoint endPoint, Func<Session> sessionFactory)
        {
            Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            this.sessionFactory = sessionFactory;

            SocketAsyncEventArgs args = new SocketAsyncEventArgs();

            args.Completed += OnConnectComplected;
            args.RemoteEndPoint = endPoint;
            args.UserToken = socket;

            RegisterConnect(args);
        }

        void RegisterConnect(SocketAsyncEventArgs args)
        {
            Socket socket = args.UserToken as Socket;
            if (socket == null)
                return;

            bool pending = socket.ConnectAsync(args);
            if (!pending)
                OnConnectComplected(null, args);
        }

        void OnConnectComplected(object sender, SocketAsyncEventArgs args)
        {
            if(args.SocketError == SocketError.Success)
            {
                Session session = sessionFactory.Invoke();
                session.Start(args.ConnectSocket);
                session.OnConnected(args.RemoteEndPoint);
            }
            else
            {
                Console.WriteLine($"OnConnectComplected Failed: {args.SocketError}");
            }
        }
    }
}
