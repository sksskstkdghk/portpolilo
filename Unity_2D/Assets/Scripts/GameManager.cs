using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : SingletonBase<GameManager>
{
    private bool isGameOver;

    private void Awake()
    {
        Init();
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void Init()
    {
        isGameOver = false;

        DontDestroyOnLoad(this);
        FindObjectOfType<PuzzleMap>().Init(8, 9);

        DataManager.Instance.Init();
        ObjectPool.Instance.Init(8, 9);
        UIManager.Instance.Init();
        SoundManager.Instance.Init();
    }

    public bool IsGameOver
    {
        set { isGameOver = value; }
        get { return isGameOver; }
    }
}
