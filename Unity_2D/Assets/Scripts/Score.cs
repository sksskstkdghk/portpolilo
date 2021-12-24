using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Score : UI
{
    private Text text;
    private int score;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public override void Init()
    {
        if (text == null)
            text = GetComponent<Text>();

        score = 0;
    }

    public override void UserUpdate()
    {
        text.text = score.ToString();
    }

    public void ScorePlus(int temp) { score += temp; }
}
