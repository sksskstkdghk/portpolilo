using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Timer : UI
{
    private Text text;
    private float gameTime;

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
        gameTime = PublicTemp.GamePlayTime;

        if (text == null)
            text = GetComponent<Text>();

        GameManager.Instance.IsGameOver = false;
    }

    public override void UserUpdate()
    {
        if (gameTime > 0.0f)
            gameTime -= Time.deltaTime;
        else
        {
            gameTime = 0.0f;
            GameManager.Instance.IsGameOver = true;
        }

        text.text = ((int)gameTime).ToString();
    }

    public float GameTime { set { gameTime = value; } }
}
