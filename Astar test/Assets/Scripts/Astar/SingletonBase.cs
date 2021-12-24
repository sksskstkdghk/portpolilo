using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class SingletonBase<T> : MonoBehaviour where T : MonoBehaviour
{
    private static T instance;

    public static T Instance
    {
        get
        {
            if (instance == null)
            {
                var obj = FindObjectOfType<T>();

                if(obj == null)
                {
                    obj = new GameObject(typeof(T).ToString()).AddComponent<T>();
                }

                instance = obj;
            }

            return instance;
        }
        
    }
}
