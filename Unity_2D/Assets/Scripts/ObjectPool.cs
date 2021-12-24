using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectPool : SingletonBase<ObjectPool>
{
    public GameObject OriginalAni;

    private List<AniPang_Component> nordList;

    private int X, Y;
    private int cursor;

    private bool isFix = false; //살아있는 노드들 위치 변경

    private float createTimer;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (GameManager.Instance.IsGameOver)
            return;

        foreach (AniPang_Component iter in nordList)
            iter.UserUpdate();

        if (isFix)
        {
            NordFix();
        }

        if (createTimer <= 0.0f && createTimer > -1.0f)
        {
            NoneNordSort();
            NoneNordInit();
        }

        if (createTimer > -1.0f)
            createTimer -= Time.smoothDeltaTime;

        //if (Input.GetKeyDown(KeyCode.F10))
        //{
        //    NoneNordSort();
        //    NoneNordInit();
        //}

        //Debug.Log(createTimer);
    }

    //게임 시작시 노드들 초기화
    //처음 시작할때 불러오는 초기화 함수
    public void Init(int x, int y)
    {
        cursor = 0;

        createTimer = 0.0f;

        X = x;
        Y = y;

        nordList = new List<AniPang_Component>(x * y);

        for(int i =0; i<x; i++)
        {
            for (int j=0; j<y; j++)
            {
                GameObject obj = Instantiate(OriginalAni, transform);
                obj.GetComponent<AniPang_Component>().Init(new Vector3(i, j, 0f));
                
                nordList.Add(obj.GetComponent<AniPang_Component>());
            }
        }
    }

    //재시작시 초기화하는 함수
    public void Init()
    {
        createTimer = 0.0f;

        int a = 0;

        for (int i = 0; i < X; i++)
            for (int j = 0; j < Y; j++)
            {
                nordList[a].Init(new Vector3(i, j, 0f));
                a++;
            }
    }

    private void NoneNordInit()
    {
        for (int i = 0; i < X; i++)
        {
            int emptySize = 0;

            for (int j = Y -1; j >= 0; j--)
            {
                RaycastHit2D hit = Ray2DNord(new Vector3(i, j, 0f));

                if (hit)
                    break;
                else
                    emptySize++;
            }

            for (int j = 0; j < emptySize; j++)
            {
                nordList[cursor].Init(new Vector2(i, j), emptySize);
                cursor++;
            }
        }

        createTimer = -1.0f;
    }

    //커서 위치(가르키는 데이터) 재설정
    //및 남아있는 노드들 위치 변환(아래로 내림)
    private void NordFix()
    {
        for(int i =0; i<X; i++)
        {
            int emptySize = 0;

            for (int j = 0; j < Y; j++)
            {
                RaycastHit2D hit = Ray2DNord(new Vector2(i, j));

                if (hit)
                {
                    if (emptySize == 0)
                        continue;

                    if (hit.collider.GetComponent<AniPang_Component>().State == AniState.WAIT)
                        hit.collider.GetComponent<AniPang_Component>().SetMoveData((float)emptySize);
                }
                else
                    emptySize++;
            }
        }

        IsFix = false;
    }

    //스크린 좌표를 월드상 좌표로 바꿔 2d에서 마우스 레이캐스트 하기 위한 함수
    public RaycastHit2D Ray2DNord(Vector2 pos)
    {
        pos += new Vector2(transform.position.x, transform.position.y);
        //pos = Camera.main.ScreenToWorldPoint(pos);
        int mask = LayerMask.GetMask("AnipangNord");

        RaycastHit2D hit = Physics2D.Raycast(pos, Vector2.zero, Mathf.Infinity, mask);

        return hit;
    }

//---------------------------------------------------Set, Get---------------------------------------------------

    public bool IsFix { set { isFix = value; } }
    public float CreateTimer { set { createTimer = value; } }

    public int Width { get { return X; } }
    public int Height { get { return Y; } }


//---------------------------------------------------Utility`---------------------------------------------------

    //NONE상태인 노드들을 맨 앞으로 이동시키는 소트
    private void NoneNordSort()
    {
        cursor = 0;
        AniPang_Component temp;

        for(int i =0; i< nordList.Count; i++)
        {
            if(nordList[i].State == AniState.NONE)
            {
                temp = nordList[i];
                nordList[i] = nordList[cursor];
                nordList[cursor] = temp;

                cursor++;
            }
        }

        cursor = 0;
    }
}
