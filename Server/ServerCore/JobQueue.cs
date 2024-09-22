using System;
using System.Collections.Generic;
using System.Text;

namespace ServerCore
{
    public interface IJobQueue
    {
        // 나중에 할 행동을 Action형태로 집어넣는 함수
        void Push(Action job);
    }

    public class JobQueue : IJobQueue
    {
        Queue<Action> jobQueue = new Queue<Action>();
        object _lock = new object();
        bool _flush = false;

        public void Push(Action job)
        {
            bool flush = false;

            lock(_lock)
            {
                jobQueue.Enqueue(job);

                if (!_flush)
                    flush = _flush = true;

            }

            if (flush)
                Flush();
        }

        void Flush()
        {
            while(true)
            {
                Action action = Pop();

                if (action == null)
                    return;

                action.Invoke();
            }
        }

        Action Pop()
        {
            lock(_lock) 
            {
                if (jobQueue.Count <= 0)
                {
                    _flush = false;
                    return null;
                }

                return jobQueue.Dequeue();
            }
        }
    }
}
