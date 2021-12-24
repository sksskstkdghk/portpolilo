using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GaugeBar : UI
{
    private RectTransform rectTransform;
    private float minGauge;

    Vector2 size;

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
        if (rectTransform == null)
        {
            rectTransform = GetComponent<RectTransform>();

            minGauge = rectTransform.sizeDelta.x / PublicTemp.GamePlayTime;
            size = new Vector2(150, 30);
        }

        gameObject.SetActive(true);
        rectTransform.sizeDelta = size;
        
    }

    public override void UserUpdate()
    {
        if (rectTransform.sizeDelta.x > 0.0f)
            rectTransform.sizeDelta -= new Vector2(minGauge * Time.deltaTime, 0f);
        else
            gameObject.SetActive(false);
    }

    public float MinGauge { set { minGauge = value; } }
}
