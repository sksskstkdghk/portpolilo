using System;
using System.Collections.Generic;
using System.Text;

// 수동(?)적인 JobQueue
// 일일히 사용자가 BroadcastTask를 클래스로 만들어 queue에 넣어주는 방식
// 이렇게 쓰는게 대체로 더 많다는데 이유는 모르겠다.(람다가 생긴지 얼마 안돼서? 라기엔 좀...)
namespace Server
{
    interface ITask
    {
        void Execute();
    }

    class BroadcastTask : ITask
    {
        GameRoom room;
        ClientSession session;
        string chat;

        BroadcastTask(GameRoom room, ClientSession session, string chat)
        {
            this.room = room;
            this.session = session;
            this.chat = chat;
        }

        public void Execute()
        {
            //room.Broadcast(session.);
        }
    }

    public class TaskQueue
    {
        Queue<ITask> queue = new Queue<ITask>();
    }
}
