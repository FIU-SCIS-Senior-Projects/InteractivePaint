using HoloToolkit;
using UnityEngine;
using System.Collections;
using HoloToolkit.Unity;

public class InteractableManager : Singleton<InteractableManager> {
     
    public GameObject FocusedGameObject { get; private set; }
    private GameObject oldFocusedGameObject = null;

	// Use this for initialization
	void Start () {
        FocusedGameObject = null;
	}
	
	// Update is called once per frame
	void Update () {
        oldFocusedGameObject = FocusedGameObject;
        //Check if gazemanager has hit something
        if (GazeManager.Instance.Hit)
        {
            //Get the ray that hit an object
            RaycastHit hitInfo = GazeManager.Instance.HitInfo;
            if(hitInfo.collider != null)
            {
                FocusedGameObject = hitInfo.collider.gameObject;
            }
            else
            {
                FocusedGameObject = null;
            }

        }
        else
        {
            FocusedGameObject = null;
        }

        if(FocusedGameObject != oldFocusedGameObject)
        {
            ResetFocusedInteractable();
            if(FocusedGameObject != null)
            {
                if(FocusedGameObject.GetComponent<Interactive>() != null)
                {
                    //Send message to object that GazeEntered -> calling that object's GazeEntered functiont
                    FocusedGameObject.SendMessage("GazeEntered");
                }
            }
        
        }
	
	}

    private void ResetFocusedInteractable()
    {
        if(oldFocusedGameObject != null)
        {
            if(oldFocusedGameObject.GetComponent<Interactive>() != null)
            {
                //Send message to old object that GazeExited
                oldFocusedGameObject.SendMessage("GazeExited");
            }
        }
    }
}
