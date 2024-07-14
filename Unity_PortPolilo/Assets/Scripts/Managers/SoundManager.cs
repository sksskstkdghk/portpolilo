using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoundManager
{
    //荤款靛 犁积 -> AudioSource
    //澜盔       -> AudioClip
    //包按(蓖)   -> AudioListener
    AudioSource[] globalSources = new AudioSource[(int)Define.Sound.MaxCount];

    Dictionary<string, AudioClip> audioClips = new Dictionary<string, AudioClip>();

    public void Init()
    {
        GameObject go = GameObject.Find("@Sound");
        if(go == null)
        {
            go = new GameObject { name = "@Sound" };
            Object.DontDestroyOnLoad(go);

            string[] soundNames = System.Enum.GetNames(typeof(Define.Sound));
            for(int i = 0; i < soundNames.Length - 1; i++)
            {
                GameObject temp = new GameObject { name = soundNames[i] };
                globalSources[i] = temp.AddComponent<AudioSource>();
                temp.transform.parent = go.transform;
            }

            globalSources[(int)Define.Sound.Bgm].loop = true;
        }
    }

    public void Play(string path, Define.Sound type = Define.Sound.Effect, AudioSource audioSource = null, float pitch = 1.0f)
    {
        AudioClip clip = GetOrAddAudioClip(path, type);
        if (clip == null)
            return;

        Play(clip, type, audioSource, pitch);
    }

    public void Play(AudioClip clip, Define.Sound type = Define.Sound.Effect, AudioSource audioSource = null, float pitch = 1.0f)
    {
        if (!clip)
            return;

        if (type == Define.Sound.Bgm)
        {
            if (audioSource == null)
                audioSource = globalSources[(int)Define.Sound.Bgm];

            if (audioSource.isPlaying)
                audioSource.Stop();

            audioSource.pitch = pitch;
            audioSource.clip = clip;
            audioSource.Play();
        }
        else
        {
            if (audioSource == null)
                audioSource = globalSources[(int)Define.Sound.Effect];

            audioSource.pitch = pitch;
            audioSource.PlayOneShot(clip);
        }
    }

    AudioClip GetOrAddAudioClip(string path, Define.Sound type = Define.Sound.Effect)
    {
        if (path.Contains("Sound/") == false)
            path = $"Sound/{path}";

        AudioClip clip = null;

        if (type == Define.Sound.Bgm)
        {
            clip = Managers._ResManager.Load<AudioClip>(path);
        }
        else
        {
            if (!audioClips.TryGetValue(path, out clip))
            {
                clip = Managers._ResManager.Load<AudioClip>(path);
                audioClips.Add(path, clip);
            }
        }

        if (clip == null)
            Debug.Log($"AudioClip Missing ! {path}");

        return clip;
    }

    public void Clear()
    {
        foreach (AudioSource item in globalSources)
        {
            item.clip = null;
            item.Stop();
        }

        audioClips.Clear();
    }
}
