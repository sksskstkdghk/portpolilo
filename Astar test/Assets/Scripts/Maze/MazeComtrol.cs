using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MazeComtrol : MonoBehaviour
{
    private List<Vector2> omnidirectional;  //전 방향

    private AstarNode start, dest; //current;
    public AstarNode current;
    public GameObject startMark, destMark;

    private Vector2 dir;

    private Vector2 startPos, destPos;
    private Vector2 min, max;

    // Start is called before the first frame update
    void Start()
    {
        omnidirectional = new List<Vector2>(4);

        //상, 하, 좌, 우
        omnidirectional.Add(new Vector2(1, 0));
        omnidirectional.Add(new Vector2(0, 1));
        omnidirectional.Add(new Vector2(-1, 0));
        omnidirectional.Add(new Vector2(0, -1));

        start = null;
        dest = null;
        current = null;

        dir = Vector2.zero;
    }

    // Update is called once per frame
    void Update()
    {
        if(!start && !dest)
            SetupMazeInit();

        if(current)
        {
            startPos = start.transform.localPosition;
            destPos = dest.transform.localPosition;

            min = new Vector2((startPos.x < destPos.x) ? startPos.x : destPos.x, (startPos.y < destPos.y) ? startPos.y : destPos.y);
            max = new Vector2((startPos.x > destPos.x) ? startPos.x : destPos.x, (startPos.y > destPos.y) ? startPos.y : destPos.y);

            MakeMaze();

            dest.NordState = State.ROAD;
        }
    }

    private void SetupMazeInit()
    {
        AstarControl controler = FindObjectOfType<AstarControl>();

        if(controler.start && controler.dest)
        {
            Vector2 closeVec = Vector2.zero;

            foreach(Vector2 iter in omnidirectional)
            {
                Vector2 temp = iter + (Vector2)controler.start.transform.localPosition;
                Vector2 destPos = (Vector2)controler.dest.transform.localPosition;

                if (closeVec == Vector2.zero ||
                   Vector2.Distance(closeVec, destPos) > Vector2.Distance(temp, destPos))
                    closeVec = temp;
            }

            start = AstarNodes.Instance.GetNord((int)closeVec.x, (int)closeVec.y);
            startMark.transform.position = start.transform.position;

            closeVec = Vector2.zero;

            foreach (Vector2 iter in omnidirectional)
            {
                Vector2 temp = iter + (Vector2)controler.dest.transform.localPosition;
                Vector2 startPos = (Vector2)start.transform.localPosition;

                if (closeVec == Vector2.zero ||
                   Vector2.Distance(closeVec, startPos) > Vector2.Distance(temp, startPos))
                    closeVec = temp;
            }

            dest = AstarNodes.Instance.GetNord((int)closeVec.x, (int)closeVec.y);
            destMark.transform.position = dest.transform.position;

            current = start;
        }
    }

    private void MakeMaze()
    {
        List<AstarNode> noneNords = new List<AstarNode>();

        foreach (Vector2 iter in omnidirectional)
        {
            Vector2 vec = iter + (Vector2)current.transform.localPosition;

            if (min.x > vec.x || min.y > vec.y ||
                max.x < vec.x || max.y < vec.y ||
                AstarNodes.Instance.GetNord((int)vec.x, (int)vec.y).NordState != State.NONE)
                continue;

            noneNords.Add(AstarNodes.Instance.GetNord((int)vec.x, (int)vec.y));
        }

        while(noneNords.Count > 0)
        {
            int random = Random.Range(0, noneNords.Count);

            if(noneNords[random].NordState != State.NONE)
            {
                noneNords.RemoveAt(random);
                continue;
            }

            Vector3 currentPos = current.transform.localPosition;

            //랜덤 주변 논 노드 탐색
            Vector2 vec = noneNords[random].transform.localPosition - currentPos;
            AstarNode next = null;

            ChangeWall(currentPos, vec);

            dir = vec;

            noneNords[random].parentNord = current;
            current.NordState = State.CLOSED;
            current = noneNords[random];

            noneNords.RemoveAt(random);

            MakeMaze();
        }

        if (current.parentNord != null)
        {
            current.NordState = State.CLOSED;
            current = current.parentNord;
        }
    }

    private void ChangeWall(Vector3 currentPos, Vector2 vec)
    {
        List<Vector2> dirList = FindObjectOfType<AstarControl>().omnidirectional;
        Vector2 nextFrontPos = (Vector2)currentPos + (vec * 2);

        int size = 0;

        if (dir != Vector2.zero && Vector2.Distance(Vector2.zero, dir - vec) > 0.9f)
            size = dirList.Count;
        else
            size = 4;

        for (int i = 0; i < size; i++)
        {
            Vector2 sideChange = (Vector2)currentPos + dirList[i];

            if (sideChange.x < min.x || sideChange.y < min.y || sideChange.x > max.x || sideChange.y > max.y)
                continue;

            AstarNode temp = AstarNodes.Instance.GetNord((int)currentPos.x + (int)dirList[i].x, (int)currentPos.y + (int)dirList[i].y);

            if (i >= 4)
            {
                if (AstarNodes.Instance.GetNord((int)nextFrontPos.x, (int)nextFrontPos.y).NordState == State.WALL ||
                    nextFrontPos.x < min.x || nextFrontPos.y < min.y || nextFrontPos.x > max.x || nextFrontPos.y > max.y)
                {
                    if (vec.x == 0)
                    {
                        if (sideChange == (Vector2)currentPos + vec + Vector2.up || sideChange == (Vector2)currentPos + vec + Vector2.down)
                            continue;
                    }
                    else if (vec.y == 0)
                    {
                        if (sideChange == (Vector2)currentPos + vec + Vector2.left || sideChange == (Vector2)currentPos + vec + Vector2.right)
                            continue;
                    }

                }
            }

            if (vec != dirList[i] && -vec != dirList[i] &&
                temp.NordState == State.NONE)
            {
                temp.NordState = State.WALL;
            }
        }
    }
}
