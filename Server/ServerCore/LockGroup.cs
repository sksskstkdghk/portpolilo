using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace LockClass
{
    public class UserSpinLock
    {
        volatile int _locked = 0;   //0: 사용가능, 1: 사용 중

        public void Acquire()
        {
            while (true)
            {
                //int original = Interlocked.Exchange(ref _locked, 1);

                //if (original == 0)
                //    break;

                //CAS Compare-And-Swap
                int expected = 0;
                int desired = 1;
                if (Interlocked.CompareExchange(ref _locked, desired, expected) == expected)
                    break;

                //조금 늦추기
                //Thread.Sleep(100); //ms단위로 기다린 뒤 다시 실행
                //Thread.Sleep(0); // 조건부 양보 => 나보다 우선순위가 낮은 애들한테는 양보x
                //Thread.Yield(); // 관대한 양보 => 지금 실행이 가능한 쓰레드가 있다면 양보
            }

        }

        public void Release()
        {
            _locked = 0;
        }
    }

    public class EventLock
    {
        //bool <- 커널에서 관리하는 변수가 있다고 생각하면 된다.
        AutoResetEvent _auto = new AutoResetEvent(true); //true: 입장가능(선착순?), false: 입장불가능
        ManualResetEvent _manual = new ManualResetEvent(true);

        public void Acquire()
        {
            _auto.WaitOne(); //입장 시도(아래 reset과정 포함)
            //_auto.Reset(); //bool = false

            //_manual.WaitOne(); //입장시도(아래 reset과정 포함x)
            //_manual.Reset();
        }

        public void Release()
        {
            _auto.Set(); //bool = true

            //_manual.Set();
        }
    }

    // 재귀적 락을 허용하는가? (Yes)
    // 스핀락 정책(5000번 -> Yield)
    public class ReaderWriterLock
    {
        const int EMPTY_FLAG = 0x00000000;
        const int WRITE_MASK = 0x7fff0000;
        const int READ_MASK = 0x0000ffff;
        const int MAX_SPIN_COUNT = 5000;

        // [Unused(1)] [WriteThreadID(15)] [ReadCount[16]]
        int flag = EMPTY_FLAG;
        int writerCount = 0;

        public void WriteLock()
        {
            // 동일 스레드가 이미 WriterLock을 이미 획득하고 있는가??
            int lockThreadID = (flag & WRITE_MASK) >> 16;
            if(Thread.CurrentThread.ManagedThreadId == lockThreadID)
            {
                writerCount++;
                return;
            }

            // 아무도 WriteLock or ReadLock을 획득하고 있지 않을 때, 경합해서 소유권을 얻는다.
            int desired = (Thread.CurrentThread.ManagedThreadId << 16) & WRITE_MASK;
            while(true)
            {
                for(int i =0; i<MAX_SPIN_COUNT; i++)
                {
                    //접근 시도
                    if (Interlocked.CompareExchange(ref flag, desired, EMPTY_FLAG) == EMPTY_FLAG)
                    {
                        writerCount = 1;
                        return;
                    }
                }

                Thread.Yield();
            }
        }

        public void WriteUnLock()
        {
            int lockCount = -writerCount;
            if (lockCount == 0)
                Interlocked.Exchange(ref flag, EMPTY_FLAG);
        }

        public void ReadLock()
        {
            int lockThreadID = (flag & WRITE_MASK) >> 16;
            if (Thread.CurrentThread.ManagedThreadId == lockThreadID)
            {
                Interlocked.Increment(ref flag);
                return;
            }

            //아무도 WriteLock을 획득하고 있지 않으면 ReadCount + 1
            while (true)
            {
                for (int i = 0; i < MAX_SPIN_COUNT; i++)
                {
                    int expected = (flag & READ_MASK);

                    if (Interlocked.CompareExchange(ref flag, expected + 1, expected) == expected)
                        return;
                }

                Thread.Yield();
            }
        }

        public void ReadUnLock()
        {
            Interlocked.Decrement(ref flag);
        }
    }
}
