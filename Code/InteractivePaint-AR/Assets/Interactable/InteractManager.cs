using UnityEngine;
using System.Collections;
using HoloToolkit.Unity;

public class InteractManager : Singleton<InteractManager> {

    public GameObject FocusedHologram { get; private set; }
    private GameObject prevHologram = null;

	// Use this for initialization
	void Start () {
        FocusedHologram = null;
	}
	
	// Update is called once per frame
	void Update () {
        prevHologram = FocusedHologram;

        if (GazeManager.Instance.Hit)
        {
            RaycastHit hitInfo = GazeManager.Instance.HitInfo;

            FocusedHologram = (hitInfo.collider != null) ? hitInfo.collider.gameObject : null;
        }
        else
        {
            FocusedHologram = null;
        }

        if(FocusedHologram != prevHologram)
        {
            SetInteractableFocus(prevHologram, "GazeExited");
            SetInteractableFocus(FocusedHologram, "GazeEntered");
        }

    }

    private void SetInteractableFocus(GameObject obj, string arg)
    {
        if(obj != null)
        {
            if(obj.GetComponent<Interactions>() != null)
            {
                obj.SendMessage(arg);
            }
        }
    }
}
