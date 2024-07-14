using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_Base : MonoBehaviour
{
    private Dictionary<Type, List<UnityEngine.Object>> UI_objects = new Dictionary<Type, List<UnityEngine.Object>>();

    //중복 오브젝트 체크 혹은 이름 체크가 없으니 알잘딱 쓸 것
    public void Bind<T>(Type type) where T : UnityEngine.Object
    {
        string[] names = Enum.GetNames(type);

        //같은 타입에 키 값이 없을 경우에만 넣기
        if (!UI_objects.ContainsKey(typeof(T)))
            UI_objects.Add(typeof(T), new List<UnityEngine.Object>(names.Length));

        for(int i =0; i<names.Length; i++)
        {
            T temp;
            temp = Utils.FindChild<T>(gameObject, names[i]);
            
            if(temp)
            {
                UI_objects[typeof(T)].Add(temp);
#if MY_DEBUG
                Debug.Log($"Bind sucess {typeof(T).Name} : {names[i]}");
#endif
            }
            else
            {
#if MY_DEBUG
                Debug.LogError($"Bind failed {typeof(T).Name} : {names[i]} is NULL");
#endif
            }

            //if (typeof(T) != typeof(GameObject))
            //{
            //    
            //}
            //else
            //{
            //    GameObject temp;
            //    temp = Utils.FindChild(gameObject, names[i]);
            //    UI_objects[typeof(T)].Add(temp);
            //}
        }
    }

    public T Get<T>(int index) where T : UnityEngine.Object
    {
        if (UI_objects.TryGetValue(typeof(T), out List<UnityEngine.Object> temp))
            return temp[index] as T;

        return null;
    }

    public Button GetBtn(int index) { return Get<Button>(index); }
    public Image  GetImg(int index) { return Get<Image>(index);  }
    public Text   GetTxt(int index) { return Get<Text>(index);   }

}
