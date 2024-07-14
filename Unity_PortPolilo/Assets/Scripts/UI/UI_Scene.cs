using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI_Scene : MonoBehaviour
{
    public virtual void Init()
    {
        Managers._UIManager.SetCanvas(gameObject, false);
    }
}
