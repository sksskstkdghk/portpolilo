using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoundManager : SingletonBase<SoundManager>
{
    [SerializeField]
    private GameObject soundNord;

    private int cursor;
    private List<AudioSource> soundList;


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void LoopFalseActiveEnd()
    {
        foreach(AudioSource iter in soundList)
        {
            if (iter.loop == false &&
                !iter.isPlaying)
                iter.gameObject.SetActive(false);
        }
    }

    public void AddPlayClip(AudioClip aClip , bool loop = false)
    {
        LoopFalseActiveEnd();

        soundList[cursor].clip = aClip;
        soundList[cursor].loop = loop;
        soundList[cursor].gameObject.SetActive(true);
        cursor++;

        if(cursor >= soundList.Count || //다음 노드가 없거나
           soundList[cursor].gameObject.activeSelf == true) //다음 노드가 재생중이라면
        {
            NoneNordSort(); //꺼져있는 사운드들 앞으로 이동
        }
    }

    public void Init()
    {
        cursor = 0;
        soundList = new List<AudioSource>(11);

        for(int i =0; i<11; i++)
        {
            GameObject temp = Instantiate(soundNord);
            temp.transform.parent = transform;
            temp.SetActive(false);
            soundList.Add(temp.GetComponent<AudioSource>());
        }

        AddPlayClip(Resources.Load<AudioClip>("Sound/anipang_ingame"), true);
    }

    private void NoneNordSort()
    {
        cursor = 0;
        AudioSource temp;

        for (int i = 0; i < soundList.Count; i++)
        {
            if (soundList[i].gameObject.activeSelf == false)
            {
                temp = soundList[i];
                soundList[i] = soundList[cursor];
                soundList[cursor] = temp;

                cursor++;
            }
        }

        cursor = 0;
    }
}
