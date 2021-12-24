using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameControler : Mouse
{
    RaycastHit2D? hit;

    //선택한 노드, 위치를 바꿀 노드
    AniPang_Component click, target;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        MouseCheck();

        if (isClick)
            ClickComponent();
    }

    public void ClickComponent()
    {
        int mask = LayerMask.GetMask("AnipangNord");

        Ray2D ray = new Ray2D(clickPos, Vector2.zero);
        hit = Physics2D.Raycast(ray.origin, ray.direction, Mathf.Infinity, mask);

        if (hit.Value)
        {
            if (hit.Value.collider.GetComponent<AniPang_Component>().State != AniState.WAIT)
                return;

            Vector2 dir = ChangeDir();

            if (dir == Vector2.zero)
                return;

            ray = new Ray2D((Vector2)hit.Value.transform.position + dir, Vector2.zero);
            RaycastHit2D changeTarget = Physics2D.Raycast(ray.origin, ray.direction, Mathf.Infinity, mask);

            if(changeTarget)
                StartCoroutine(SeatChange(changeTarget, dir));

            isClick = false;

            //Debug.Log(hit.Value.collider.gameObject.name);
        }
    }

    private IEnumerator SeatChange(RaycastHit2D changeTarget, Vector2 dir)
    {
        click = hit.Value.collider.gameObject.GetComponent<AniPang_Component>();
        target = changeTarget.collider.gameObject.GetComponent<AniPang_Component>();

        click.SetMoveData(1.0f, dir);
        target.SetMoveData(1.0f, -dir);

        for (float i = 1.0f; i > -1.0f; i -= Time.deltaTime * PublicTemp.speed)
        {
            if(i <= -0.9f)
            {
                i = -1.0f;

                if (click.State != AniState.WAIT || target.State != AniState.WAIT)
                    break;

                click.SetMoveData(1.0f, -dir);
                target.SetMoveData(1.0f, dir);
                break;
            }

            yield return null;
        }
    }
}
