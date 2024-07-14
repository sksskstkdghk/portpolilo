using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI_Popup : UI_Base
{
    public virtual void Init()
    {
        Managers._UIManager.SetCanvas(gameObject);
    }

    public virtual void ClosePopupUI()
    {
        Managers._UIManager.ClosePopupUI(this);
    }
}
