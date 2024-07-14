using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public abstract class BaseScene : MonoBehaviour
{
    Define.Scene sceneType = Define.Scene.Unknown;

    public Define.Scene SceneType { get { return sceneType; } protected set { sceneType = value; } }

    public void Start()
    {
        Init();
    }

    protected virtual void Init()
    {
        Object obj = GameObject.FindObjectOfType(typeof(EventSystem));
        if(obj == null)
            obj = Managers._ResManager.Instantiate("UI/EventSystem");

        obj.name = "@EvenySystem";
    }

    public abstract void Clear();

}
