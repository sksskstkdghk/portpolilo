using System;
using System.Collections.Generic;
using System.Text;

namespace ServerCore
{
    public class PriorityQueue<T> where T : IComparable<T>
    {
        List<T> heap = new List<T>();

        public int Count { get { return heap.Count; } }

        public void Push(T data)
        {
            heap.Add(data);

            int now = heap.Count - 1;

            while (now > 0)
            {
                int next = (now - 1) / 2;

                if (heap[now].CompareTo(heap[next]) < 0)
                    break;

                T temp = heap[now];
                heap[now] = heap[next];
                heap[next] = temp;

                now = next;
            }
        }

        public T Pop()
        {
            T ret = heap[0];
            int lastIndex = heap.Count - 1;

            heap[0] = heap[lastIndex];
            heap.RemoveAt(lastIndex);
            lastIndex--;

            int now = 0;

            while (true)
            {
                int left = 2 * now + 1;
                int right = 2 * now + 2;

                int next = now;

                if (left <= lastIndex && heap[next].CompareTo(heap[left]) < 0)
                    next = left;

                if (right <= lastIndex && heap[next].CompareTo(heap[right]) < 0)
                    next = right;

                if (next == now)
                    break;

                T temp = heap[now];
                heap[now] = heap[next];
                heap[next] = temp;

                now = next;
            }

            return ret;
        }

        public T Peek()
        {
            if (heap.Count <= 0)
                return default(T);

            return heap[0];
        }
    }
}
