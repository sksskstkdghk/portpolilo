using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AniPang_Component : MonoBehaviour
{
    private Vector2 dir, movePos;   //이동방향, 이동 위치

    private AniCategory category;   //어떤 종족인가?
    private AniState    state;      //현재 상태

    private bool        isCheck;    //한 번만 주변을 체크하기 위한 bool값

    private RaycastHit2D hit;

    // Start is called before the first frame update
    void Start()
    {
        
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void UserUpdate()
    {
        TestFunc();

        SetState();
    }

    private void TestFunc()
    {
        
        
    }

    //상태별 행동
    private void SetState()
    {
        //터지고 난후
        if (state == AniState.NONE)
        {
            ObjectPool.Instance.IsFix = true;
            gameObject.SetActive(false);

            Debug.Log("none");
        }

        //터질때
        if (state == AniState.PANG)
        {
            Pang();
        }
        //이동할때
        else if (state == AniState.MOVE)
        {
            Move();

            isCheck = true;

            //이동 중에는 빈 자리를 채우지 않게함
            ObjectPool.Instance.CreateTimer = 0.3f;
        }
        //정지상태일때
        else if (state == AniState.WAIT)
        {
            Wait();
        }

        //Debug.Log(state);
    }

    //3개 이상인지 확인
    private void CheckMoreThanThree()
    {
        //같은 방향별로 저장
        List<AniPang_Component>  upDown = new List<AniPang_Component>();
        List<AniPang_Component>  leftRight = new List<AniPang_Component>();

        upDown.Add(gameObject.GetComponent<AniPang_Component>());   //자기 자신 저장
        leftRight.Add(gameObject.GetComponent<AniPang_Component>());    //자기 자신 저장

        //전방향 탐색(상, 하, 좌, 우)
        foreach (Vector2 iter in DataManager.Instance.Around)
        {
            //상, 하 탐색(DFS)
            if (iter == Vector2.up || iter == Vector2.down)
                GetComponent<AniPang_Component>().OneWayCheck(iter, upDown);
            //좌, 우 탐색(DFS)
            else
                GetComponent<AniPang_Component>().OneWayCheck(iter, leftRight);
        }

        if (upDown.Count >= 3)  //상, 하 탐색 후 3개(자기포함) 이상이면
        {
            foreach (AniPang_Component iter in upDown)
                iter.state = AniState.PANG; //터트림
        }

        if (leftRight.Count >= 3)   //좌, 우 탐색 후 3개(자기포함) 이상이면
        {
            foreach (AniPang_Component iter in leftRight)
                iter.state = AniState.PANG; //터트림
        }  

        if (upDown.Count >= 3 ||
           leftRight.Count >= 3)
            SoundManager.Instance.AddPlayClip(Resources.Load<AudioClip>("Sound/sound 19"));
    }

    //단 방향 같은 노드 갯수 확인
    public void OneWayCheck(Vector2 dir, List<AniPang_Component> list)
    {
        int mask = LayerMask.GetMask("AnipangNord");

        //자기자신으로 부터 방향에 따른 다음 노드 탐색
        Ray2D ray = new Ray2D((Vector2)transform.position + dir, Vector2.zero);
        RaycastHit2D hit = Physics2D.Raycast(ray.origin, ray.direction, Mathf.Infinity, mask);

        if (hit.collider == null)
            return;

        AniPang_Component temp = hit.collider.gameObject.GetComponent<AniPang_Component>();

        //같은 종족이고 이동상태가 아니라면
        if (temp.Category == category &&
            temp.State != AniState.MOVE)
        {
            //저장하고 재귀
            list.Add(temp);
            temp.OneWayCheck(dir, list);
        }
    }

    public void Init(Vector3 pos, float dis = 9.0f)
    {
        //초기 위치값 설정
        float startPos = ObjectPool.Instance.Height;
        transform.localPosition = new Vector3(pos.x, startPos + pos.y, 0f);
        //위에서 아래로 떨어짐
        SetMoveData(dis);

        //이미지(종족) 랜덤 변경
        int ran = Random.Range(0, DataManager.Instance.AnimalSize);
        category = (AniCategory)ran;
        //랜덤값에 따른 종족 이미지 설정
        GetComponent<SpriteRenderer>().sprite = DataManager.Instance.GetSprite(ran);
        //이미지 별 크기 설정
        transform.localScale = DataManager.Instance.GetSpriteSize(ran);
        //콜라이더 크기 설정
        GetComponent<BoxCollider2D>().size = new Vector2((0.9f / transform.localScale.x), (0.9f / transform.localScale.y));

        //Debug.Log(ran);

        gameObject.SetActive(true);
    }

    public void SetMoveData(float dis, Vector2? dir = null)
    {
        if(dir == null)
            this.dir = Vector2.down;
        else
            this.dir = dir.Value;

        movePos = (Vector2)transform.localPosition + (this.dir * dis);

        state = AniState.MOVE;
    }

    private void Move()
    {
        if (Vector2.Distance(transform.localPosition, movePos) <= 0.1f)
        {
            transform.localPosition = movePos;
            state = AniState.WAIT;
            return;
        }

        transform.localPosition += (Vector3)dir * Time.deltaTime * PublicTemp.speed;
    }

    private void Wait()
    {
        if (isCheck)
        {
            CheckMoreThanThree();
            isCheck = false;
        }
    }

    //3개이상이 되어 터짐
    public void Pang()
    {
        state = AniState.NONE;

        Score temp = FindObjectOfType<Score>(); //점수 UI탐색
        temp.ScorePlus(100);  //점수추가

        Debug.Log("pang");
    }

//---------------------------------------Set, Get-----------------------------------------------------

    public AniCategory Category { get { return category; } }
    public AniState    State    { get { return state; } }

}