using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public static class Extension
{
    static public T GetOrAddComponent<T>(this GameObject parent) where T : UnityEngine.Component
    {
        T temp = parent.GetComponent<T>();

        if (temp == null)
            temp = parent.AddComponent<T>();

        return temp;
    }
}
