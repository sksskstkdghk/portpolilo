using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SingletonBase<T> : MonoBehaviour where T : MonoBehaviour
{
    private static T instance = null;

    public static T Instance
    {
        get
        {
            var obj = FindObjectOfType<T>();

            if (obj == null)
            {
                Create();
            }
            else
                instance = obj;

            return instance;
        }
    }

    public static void Create()
    {
        if (instance == null)
        {
            instance = new GameObject(typeof(T).ToString()).AddComponent<T>();
        }
    }
}
