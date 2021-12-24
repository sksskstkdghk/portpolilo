using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Mouse : MonoBehaviour
{
    protected Vector2 clickPos, change;
    protected bool isClick = false;

    protected void MouseCheck()
    {
        if (Input.GetMouseButtonUp(0))
            isClick = false;

        if (Input.GetMouseButtonDown(0))
        {
            clickPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            isClick = true;
        }

        if(isClick && Input.GetMouseButton(0))
        {
            Vector2 pressPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);

            change = clickPos - pressPos;
        }
    }

    protected Vector2 ChangeDir()
    {
        if (change.x >= 0.5f)
            return Vector2.left;
        else if (change.x <= -0.5f)
            return Vector2.right;
        else if (change.y >= 0.5f)
            return Vector2.down;
        else if (change.y <= -0.5f)
            return Vector2.up;
        else
            return Vector2.zero;
    }
}
