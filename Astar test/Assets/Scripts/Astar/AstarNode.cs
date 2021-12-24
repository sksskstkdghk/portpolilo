using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum State
{
    NONE = 0,
    WALL, ROAD, OPEN, CLOSED,


}


public class AstarNode : MonoBehaviour
{
    [SerializeField]
    private State state;
    private Material material = null;

    public AstarNode parentNord;

    //각각, 비용, 시작지점부터의 거리, 도착지점까지의 거리
    public int F, G, H;
    private TextMesh textF, textG, TextH;

    // Update is called once per frame
    void Update()
    {
        if (state == State.NONE)
            material.color = new Color(1.0f, 1.0f, 1.0f);
        else if (state == State.CLOSED)
            material.color = new Color(0.0f, 0.0f, 0.0f);
        else if (state == State.OPEN)
            material.color = new Color(0.0f, 0.0f, 1.0f);
        else if (state == State.ROAD)
            material.color = new Color(1.0f, 1.0f, 0.0f);
        else if (state == State.WALL)
            material.color = new Color(1.0f, 0.0f, 0.0f);
    }

    public State NordState
    {
        set { state = value; }
        get { return state;  }
    }

    public void Init()
    {
        state = State.NONE;
        F = G = H = 0;

        if (material)
            return;

        material = GetComponent<SpriteRenderer>().material;

        parentNord = null;
    }

    public void SetG(AstarNode parent, Vector2? dir = null)
    {
        if(dir == null)
            dir = parent.transform.localPosition - transform.localPosition;

        int temp;

        if ((int)dir.Value.x == 0 || (int)dir.Value.y == 0)
            temp = parent.G + PublicTemp.nordStraightDistanceSize;
        else
            temp = parent.G + PublicTemp.nordDiagonalDistanceSize;

        if (G > temp || parentNord == null)
        {
            parentNord = parent;
            G = temp;
        }
    }
}
