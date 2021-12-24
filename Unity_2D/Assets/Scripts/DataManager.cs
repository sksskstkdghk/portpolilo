using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DataManager : SingletonBase<DataManager>
{
    private List<SpriteData> spriteData;
    private List<Vector2> around;

    public void Init()
    {
        spriteData = new List<SpriteData>();
        around = new List<Vector2>();

        spriteData.Add(new SpriteData(Resources.Load<Sprite>("Animal/ani01"), new Vector3(2.8f, 3.5f, 1.0f)));
        spriteData.Add(new SpriteData(Resources.Load<Sprite>("Animal/ani02"), new Vector3(3.5f, 3.0f, 1.0f)));
        spriteData.Add(new SpriteData(Resources.Load<Sprite>("Animal/ani03"), new Vector3(4.5f, 4.5f, 1.0f)));
        spriteData.Add(new SpriteData(Resources.Load<Sprite>("Animal/ani04"), new Vector3(4.0f, 4.0f, 1.0f)));
        spriteData.Add(new SpriteData(Resources.Load<Sprite>("Animal/ani05"), new Vector3(3.5f, 3.5f, 1.0f)));

        around.Add(Vector2.up);
        around.Add(Vector2.right);
        around.Add(Vector2.down);
        around.Add(Vector2.left);
    }

    public Sprite GetSprite(int index)
    {
        return spriteData[index].img;
    }

    public Vector3 GetSpriteSize(int index)
    {
        return spriteData[index].size;
    }

    public SpriteData GetData(int index)
    {
        return spriteData[index];
    }

//----------------------------------------------Set, Get-----------------------------------------------------

    public int AnimalSize { get { return spriteData.Count; } }
    public List<Vector2> Around { get { return around; } }
}
