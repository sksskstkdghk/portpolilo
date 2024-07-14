using System.Collections; 
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Test : MonoBehaviour
{

    // Start is called before the first frame update
    void Start()
    {
        Managers._SoundManager.Play("Sound/1 Person Cheering Sound", audioSource : gameObject.GetComponent<AudioSource>());
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        
    }
}
