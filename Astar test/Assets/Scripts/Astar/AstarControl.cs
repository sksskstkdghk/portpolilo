using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AstarControl : MonoBehaviour
{
    public List<Vector2> omnidirectional;  //전 방향

    //출발, 도착, 현재 노드
    public AstarNode start, dest, current;
    public GameObject startMark, destMark, currentMark;

    private List<AstarNode> openList;

    private int mask;
    private bool check;

    // Start is called before the first frame update
    void Start()
    {
        omnidirectional = new List<Vector2>(8);

        //상, 하, 좌, 우
        omnidirectional.Add(new Vector2(1, 0));
        omnidirectional.Add(new Vector2(0, 1));
        omnidirectional.Add(new Vector2(-1, 0));
        omnidirectional.Add(new Vector2(0, -1));

        //대각선
        omnidirectional.Add(new Vector2(1, 1));
        omnidirectional.Add(new Vector2(-1, 1));
        omnidirectional.Add(new Vector2(-1, -1));
        omnidirectional.Add(new Vector2(1, -1));

        mask = LayerMask.GetMask("Nord");

        start = dest = null;
        check = false;

        openList = new List<AstarNode>();
    }

    // Update is called once per frame
    void Update()
    {
        SelectNord();

        //if (start && dest && check)
        //    Astar();

        //if (start && dest)
        //    Astar();
    }

    private void SelectNord()
    {
        Vector3 temp = Camera.main.ScreenToWorldPoint(Input.mousePosition);

        Ray2D ray = new Ray2D(temp, Vector2.zero);
        RaycastHit2D hit = Physics2D.Raycast(ray.origin, ray.direction, Mathf.Infinity, mask);

        if (!hit)
            return;

        if(Input.GetMouseButtonDown(0))
        {
            start = hit.collider.GetComponent<AstarNode>();
            current = start;
            current.NordState = State.OPEN;
            openList.Add(current);
            startMark.transform.position = start.transform.position;

            check = true;

            Debug.Log(hit.collider.name);
            Debug.Log(temp);
        }
        else if (Input.GetMouseButtonDown(1))
        {
            dest = hit.collider.GetComponent<AstarNode>();
            destMark.transform.position = dest.transform.position;

            Debug.Log(hit.collider.name);
            Debug.Log(temp);
        }
        else if (Input.GetMouseButtonDown(2))
        {
            hit.collider.GetComponent<AstarNode>().NordState = State.WALL;
        }
    }

    private void Astar()
    {
        //if (Input.GetKeyDown(KeyCode.F10) &&
        //    current != dest)
        //{
        //    NordFGH();
        //    CheckLowOpenG();
        //    FindCloseNord();
        //}

        while (current != dest)
        {
            NordFGH();
            CheckLowOpenG();
            FindCloseNord();
        }

        BackStart();

        check = false;
    }

    //H구하기 위한 함수
    private int ManhattanDistance(Vector2 startVec, Vector2 destVec)
    {
        Vector2 temp = destVec - startVec;

        int value = (Mathf.Abs((int)temp.x) + Mathf.Abs((int)temp.y)) * PublicTemp.nordStraightDistanceSize;

        return value;
    }

    //G구하기 위한 함수
    //private int NordDIstance(AstarNord nord)
    //{
    //    int x = (int)nord.transform.localPosition.x;
    //    int y = (int)nord.transform.localPosition.y;

    //    foreach (Vector2 iter in omnidirectional)
    //    {
    //        if (x + (int)iter.x < 0 || y + (int)iter.y < 0 ||
    //            x + (int)iter.x > 9 || y + (int)iter.y > 9)
    //            continue;

    //        AstarNord temp = AstarNords.Instance.GetNord(x + (int)iter.x, y + (int)iter.y);

    //        if (nord.NordState != State.OPEN)
    //            continue;


    //    }

    //    return value;
    //}

    private void CheckLowOpenG()
    {
        int x = (int)current.transform.localPosition.x;
        int y = (int)current.transform.localPosition.y;

        foreach (Vector2 iter in omnidirectional)
        {
            if (x + (int)iter.x < 0 || y + (int)iter.y < 0 ||
                x + (int)iter.x > 9 || y + (int)iter.y > 9)
                continue;

            AstarNode temp = AstarNodes.Instance.GetNord(x + (int)iter.x, y + (int)iter.y);

            if (temp.NordState != State.OPEN || temp.G > current.G)
                continue;

            ChangeOpenG(temp);
        }
    }

    private void ChangeOpenG(AstarNode nord)
    {
        int x = (int)nord.transform.localPosition.x;
        int y = (int)nord.transform.localPosition.y;

        foreach (Vector2 iter in omnidirectional)
        {
            if (x + (int)iter.x < 0 || y + (int)iter.y < 0 ||
                x + (int)iter.x > 9 || y + (int)iter.y > 9)
                continue;

            AstarNode temp = AstarNodes.Instance.GetNord(x + (int)iter.x, y + (int)iter.y);

            temp.SetG(nord, iter);
        }
    }

    //현재 노드에서 주변 노드 탐색전에 가중치 설정하는 함수
    private void NordFGH()
    {
        int x = (int)current.transform.localPosition.x;
        int y = (int)current.transform.localPosition.y;

        foreach (Vector2 iter in omnidirectional)
        {
            if (x + (int)iter.x < 0 || y + (int)iter.y < 0 ||
                x + (int)iter.x > 9 || y + (int)iter.y > 9)
                continue;

            AstarNode nord = AstarNodes.Instance.GetNord(x + (int)iter.x, y + (int)iter.y);

            if (nord.NordState == State.CLOSED || nord.NordState == State.WALL)
                continue;

            if(nord.NordState == State.NONE)
            {
                nord.NordState = State.OPEN;
                openList.Add(nord);
            }

            Vector2 startVec = nord.transform.localPosition;
            Vector2 destVec = dest.transform.localPosition;

            nord.H = ManhattanDistance(startVec, destVec);
            nord.SetG(current);
            nord.F = nord.G + nord.H;
        }
    }

    //현재노드에서 도착노드까지 가장 가까운 주변 노드를 탐색
    private void FindCloseNord()
    {
        int x = (int)current.transform.localPosition.x;
        int y = (int)current.transform.localPosition.y;

        AstarNode nord = null;

        foreach (Vector2 iter in omnidirectional)
        {
            if (x + (int)iter.x < 0 || y + (int)iter.y < 0 ||
                x + (int)iter.x > 9 || y + (int)iter.y > 9)
                continue;

            AstarNode temp = AstarNodes.Instance.GetNord(x + (int)iter.x, y + (int)iter.y);

            if (temp.NordState != State.OPEN)
                continue;

            if (nord == null || nord.F >= temp.F)
                nord = temp;
        }

        current.NordState = State.CLOSED;
        openList.Remove(current);

        if (!nord)
        {
            foreach (AstarNode iter in openList)
            {
                if (!nord || nord.F > iter.F)
                    nord = iter;
            }
        }

        current = nord;

        currentMark.transform.position = current.transform.position;
    }

    //오픈된 노드들 중 다시 되돌아가며 최적의 길 구함
    private void BackStart()
    {
        if(current == start)
        {
            current.NordState = State.ROAD;
        }
        else
        {
            current.NordState = State.ROAD;
            current = current.parentNord;

            BackStart();
        }
    }
}
