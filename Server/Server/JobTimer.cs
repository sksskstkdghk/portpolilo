using ServerCore;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace Server
{
    struct JobTimerElem : IComparable<JobTimerElem>
    {
        public int execTick; // 실행 시간
        public Action action;

        public int CompareTo(JobTimerElem other)
        {
            return other.execTick - execTick;
        }
    }

    public class JobTimer
    {
        PriorityQueue<JobTimerElem> pq = new PriorityQueue<JobTimerElem>();
        object _lock = new object();

        public static JobTimer Instance { get; } = new JobTimer();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="action">실행 해야할 함수</param>
        /// <param name="tickAfter">기다려야 하는 시간(단위 : ms)</param>
        public void Push(Action action, int tickAfter = 0)
        {
            JobTimerElem job;
            job.execTick = System.Environment.TickCount + tickAfter;
            job.action = action;

            lock (_lock) 
            { 
                pq.Push(job);
            }
        }

        public void Flush()
        {
            while(true)
            {
                int now = System.Environment.TickCount;

                JobTimerElem job;

                lock(_lock)
                {
                    if (pq.Count <= 0)
                        break;

                    job = pq.Peek();
                    if (job.execTick > now)
                        break;

                    pq.Pop();
                }

                job.action.Invoke();
            }
        }
    }
}
