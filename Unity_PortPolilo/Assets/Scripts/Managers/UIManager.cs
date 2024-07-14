using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIManager
{
    int order = 0;

    Stack<UI_Popup> popupStack = new Stack<UI_Popup>();
    UI_Scene sceneUI = null;

    public GameObject UI_PopopRoot
    {
        get
        {
            GameObject temp = GameObject.Find("@UI_Popup");
            if (!temp)
                temp = new GameObject() { name = "@UI_Popup" };

            return temp;
        }
    }
    public GameObject UI_SceneRoot
    {
        get
        {
            GameObject temp = GameObject.Find("@UI_Scene");
            if (!temp)
                temp = new GameObject() { name = "@UI_Scene" };

            return temp;
        }
    }


    public void SetCanvas(GameObject go, bool sort = true)
    {
        Canvas canvas = Utils.GetOrAddComponent<Canvas>(go);
        canvas.renderMode = RenderMode.ScreenSpaceOverlay;
        canvas.overrideSorting = true;

        if (sort)   //팝업
        {
            canvas.sortingOrder = order;
            order++;
        }
        else   //scene
        {
            canvas.sortingOrder = 0;
        }

    }

    public T ShowPopupUI<T>(string name = null) where T : UI_Popup
    {
        if (string.IsNullOrEmpty(name))
            name = typeof(T).Name;

        GameObject go = Managers._ResManager.Instantiate($"UI/Popup/{name}");
        T popup = Utils.GetOrAddComponent<T>(go);
        popup.Init();
        popupStack.Push(popup);

        go.transform.SetParent(UI_PopopRoot.transform);

        return popup;
    }

    public T ShowSceneUI<T>(string name = null) where T : UI_Scene
    {
        if (string.IsNullOrEmpty(name))
            name = typeof(T).Name;

        GameObject go = Managers._ResManager.Instantiate($"UI/Popup/{name}");
        T sceneUI = Utils.GetOrAddComponent<T>(go);
        sceneUI.Init();
        this.sceneUI = sceneUI;
            
        go.transform.SetParent(UI_SceneRoot.transform);

        return sceneUI;
    }

    public void ClosePopupUI()
    {
        if (popupStack.Count <= 0)
            return;

        UI_Popup temp = popupStack.Pop();
        Managers._ResManager.Destroy(temp.gameObject);
        temp = null;    //안전하게 null처리

        order--;
    }

    public void ClosePopupUI(UI_Popup popup)
    {
        if (popupStack.Count <= 0)
            return;

        if(popupStack.Peek() != popup)
            Debug.Log($"Close Popup({popup.name}) Failed!");

        ClosePopupUI();
    }

    public void CloseAllPopupUI()
    {
        while (popupStack.Count > 0)
            ClosePopupUI();
    }

    public void Clear()
    {
        CloseAllPopupUI();
        sceneUI = null;
    }
}
