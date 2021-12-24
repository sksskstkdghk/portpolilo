using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PuzzleMap : MonoBehaviour
{
    public GameObject OriginalNord;

    // Start is called before the first frame update
    void Awake()
    {
        
    }

    public void Init(int x, int y)
    {
        for (int i = 0; i < y; i++)
            for (int j = 0; j < x; j++)
            {
                GameObject obj = Instantiate(OriginalNord, gameObject.transform);
                obj.transform.localPosition = new Vector3(j, i, 0);
            }
    }
}
