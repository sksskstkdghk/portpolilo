using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AstarNodes : SingletonBase<AstarNodes>
{
    private List<List<AstarNode>> nords;
    public AstarNode original;

    // Start is called before the first frame update
    void Start()
    {
        nords = new List<List<AstarNode>>();

        for(int i =0; i<10; i++)
        {
            List<AstarNode> a = new List<AstarNode>();

            for (int j = 0; j < 10; j++)
            {
                var temp = Instantiate(original, transform);
                temp.transform.localPosition = new Vector2(i, j);
                temp.Init();

                a.Add(temp);
            }

            nords.Add(a);
        }
    }

    public void Init()
    {
        foreach(List<AstarNode> i in nords)
        {
            foreach (AstarNode j in i)
                j.Init();
        }
    }

    public AstarNode GetNord(int x, int y) { return nords[x][y]; }
}
