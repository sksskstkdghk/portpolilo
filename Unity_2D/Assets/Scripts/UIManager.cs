using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIManager : SingletonBase<UIManager>
{
    [SerializeField]
    public Dictionary<string, UI> UIList;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Menu();

        if (GameManager.Instance.IsGameOver)
            return;

        foreach(UI iter in UIList.Values)
            iter.UserUpdate();
    }

    public void Init()
    {
        if(UIList == null)
        {
            UIList = new Dictionary<string, UI>();

            int end = GetComponentsInChildren<UI>().Length;

            for(int i =0; i< end; i++)
            {
                UI temp = GetComponentsInChildren<UI>()[i];

                UIList.Add(temp.name, temp);
            }
        }

        foreach (UI iter in UIList.Values)
            iter.Init();
    }

    private void Menu()
    {
        if (GameManager.Instance.IsGameOver &&
            !UIList["Menu"].gameObject.activeSelf)
            UIList["Menu"].gameObject.SetActive(true);
    }
}
