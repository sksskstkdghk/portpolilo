using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Utils
{
    static public T FindChild<T>(GameObject parent, string childName) where T : UnityEngine.Object
    {
        if (typeof(T) != typeof(GameObject))
        {
            foreach (T item in parent.GetComponentsInChildren<T>())
            {
                if (item.name.Equals(childName))
                    return item;
            }
        }
        else
        {
            foreach (Transform item in parent.GetComponentsInChildren<Transform>())
            {
                if (item.name.Equals(childName))
                    return item.gameObject as T;
            }
        }

        return null;
    }

    static public T GetOrAddComponent<T>(GameObject parent) where T : UnityEngine.Component
    {
        T temp = parent.GetComponent<T>();

        if(temp == null)
            temp = parent.AddComponent<T>();

        return temp;
    }
}
