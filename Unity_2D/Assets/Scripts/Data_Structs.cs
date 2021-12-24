using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpriteData
{
    public Sprite img;
    public Vector3 size;

    public SpriteData(Sprite img, Vector3 size)
    {
        this.img = img;
        this.size = size;
    }
}

//각 노드들에 종족값
public enum AniCategory
{
    NONE = -1,
    ANI01, ANI02, ANI03, ANI04, ANI05
}

//각 동물노드들 상태 확인용 열거형
public enum AniState
{
    NONE = 0,
    WAIT, MOVE, PANG
}

public enum UICategory
{
    NONE = 0,
    TIMER, GAUGE
}


//전역 변수
public struct PublicTemp
{
    public static float speed = 10.0f;          //노드 속도
    public static float GamePlayTime = 60.0f;   //판 당 게임시간
}

