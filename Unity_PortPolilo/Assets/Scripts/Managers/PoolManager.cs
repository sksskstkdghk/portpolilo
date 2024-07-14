using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PoolManager
{
    class Pool
    {
        public GameObject original { get; private set; }
        public Transform root { get; set; }

        Stack<Poolable> poolStack = new Stack<Poolable>();  //현재 활동하지 않는 객체들만 모아둠

        public void Init(GameObject original, int count = 5)
        {
            this.original = original;
            root = new GameObject().transform;
            root.name = $"{original.name}_Root";

            for(int i =0; i<count; i++)
                Push(Create());
        }

        Poolable Create()
        {
            GameObject go = Object.Instantiate<GameObject>(original);
            go.name = original.name;
            return go.GetOrAddComponent<Poolable>();
        }

        public void Push(Poolable poolable)
        {
            if (!poolable)
                return;

            poolable.transform.parent = root;
            poolable.gameObject.SetActive(false);
            poolable.isUsing = false;

            poolStack.Push(poolable);
        }

        public Poolable Pop(Transform parent)
        {
            Poolable poolable;

            if (poolStack.Count > 0)
                poolable = poolStack.Pop();
            else
                poolable = Create();

            poolable.gameObject.SetActive(true);

            if (!parent)
                poolable.transform.parent = Managers._SceneManager.CurrentScene.transform;

            poolable.transform.parent = parent;
            poolable.isUsing = true;

            return poolable;
        }
    }

    Dictionary<string, Pool> pools = new Dictionary<string, Pool>();
    Transform root;

    public void Init()
    {
        if(!root)
        {
            root = new GameObject { name = "@PoolManager" }.transform;
            Object.DontDestroyOnLoad(root);
        }
    }

    public void CreatePool(GameObject original, int count = 5)
    {
        Pool pool = new Pool();
        pool.Init(original, count);
        pool.root.parent = root;

        pools.Add(original.name, pool);
    }

    public void Push(Poolable poolable)
    {
        string name = poolable.gameObject.name;

        if (!pools.ContainsKey(name))
        {
            GameObject.Destroy(poolable.gameObject);
            return;
        }

        pools[name].Push(poolable);
    }

    public Poolable Pop(GameObject original, Transform parent = null)
    {
        if(!pools.ContainsKey(original.name))
            CreatePool(original);

        return pools[original.name].Pop(parent);
    }

    public GameObject GetOriginal(string name)
    {
        if (!pools.ContainsKey(name))
            return null;

        return pools[name].original;
    }

    public void Clear()
    {
        foreach (Transform child in root)
            GameObject.Destroy(child.gameObject);

        pools.Clear();
    }

    
}
