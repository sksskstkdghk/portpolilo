using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ServerCore
{
    // 서버 연결을 담당하는 클래스(연결 요청 받기) - recv
    public class Listener
    {
        Socket listenSocket;
        Func<Session> SessionFactory;

        public void Init(IPEndPoint endPoint, Func<Session> SessionFactory)
        {
            // 문지기
            listenSocket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            this.SessionFactory += SessionFactory;

            // 문지기 교육
            listenSocket.Bind(endPoint);

            // 영업 시작
            // backlog : 최대 대기수
            listenSocket.Listen(10);

            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.Completed += new EventHandler<SocketAsyncEventArgs>(OnAcceptCompleted);
            RegisterAccept(args);
        }

        private void RegisterAccept(SocketAsyncEventArgs args)
        {
            //재사용 시 같은 소켓 사용 가능성 배제
            args.AcceptSocket = null;

            // false: 접속 성공, true: 접속 보류상태(접속이 없음? 혹은 접속 실패?)
            // AcceptAsync해당 함수가 빠르게 완료되면 false이다 아마도 내부적으로 약간 시간을 기다리는 것 같음
            // 기본적으로 비동기 방식이니 내부에서 따로 멀티 쓰레드로 처리하는건가?? 그럼 pending이 이상한데...
            bool pending = listenSocket.AcceptAsync(args);

            if (pending == false)
                OnAcceptCompleted(null, args);
        }

        private void OnAcceptCompleted(object sender, SocketAsyncEventArgs args)
        {
            if(args.SocketError == SocketError.Success) //에러 없음
            {
                Session session = SessionFactory.Invoke();
                session.Start(args.AcceptSocket);
                session.OnConnected(args.AcceptSocket.RemoteEndPoint);
            }
            else
                Console.WriteLine(args.SocketError.ToString());

            RegisterAccept(args);
        }
    }
}
