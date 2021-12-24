using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Menu : UI
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public override void Init()
    {
        gameObject.SetActive(false);
    }

    public override void UserUpdate() { }

    private void ReStart()
    {
        Init();
        ObjectPool.Instance.Init();
        UIManager.Instance.Init();
    }

    private void Exit()
    {

#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit();
#endif
    }
}
