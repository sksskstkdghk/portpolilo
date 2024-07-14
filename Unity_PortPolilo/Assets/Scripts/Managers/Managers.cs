using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Managers : MonoBehaviour
{
    static Managers instance;
    static Managers Instance { get { Init(); return instance; } }

    DataManager data = new DataManager();
    InputManager input = new InputManager();
    ResourcesManager resources = new ResourcesManager();
    UIManager ui = new UIManager();
    SceneManagerEx scene = new SceneManagerEx();
    SoundManager sound = new SoundManager();
    PoolManager pool = new PoolManager();

    public static DataManager _DataManager { get { return Instance.data; } }
    public static InputManager _InputManager { get { return Instance.input; } }
    public static ResourcesManager _ResManager { get { return Instance.resources; } }
    public static UIManager _UIManager { get { return Instance.ui; } }
    public static SceneManagerEx _SceneManager { get { return Instance.scene; } }
    public static SoundManager _SoundManager { get { return Instance.sound; } }
    public static PoolManager _PoolManager { get { return Instance.pool; } }

    public void Start()
    {
        
    }

    private void Update()
    {
        _InputManager.OnUpdate();
    }

    private static void Init()
    {
        if (!instance)
        {
            GameObject temp = GameObject.Find("@Managers");

            if (!temp)
            {
                temp = new GameObject() { name = "@Managers" };
                temp.AddComponent<Managers>();
            }

            DontDestroyOnLoad(temp);
            instance = temp.GetComponent<Managers>();

            instance.data.Init();
            instance.sound.Init();
            instance.pool.Init();
        }
    }

    public static void Clear()
    {
        _SoundManager.Clear();
        _InputManager.Clear();
        _SceneManager.Clear();
        _UIManager.Clear();
        _PoolManager.Clear();
    }
}
