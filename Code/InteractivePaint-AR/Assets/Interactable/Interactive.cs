using UnityEngine;
using System.Collections;
using HoloToolkit.Unity;

public class Interactive : MonoBehaviour {


    [Tooltip("Audio clip to play when interacting with hologram.")]
    public AudioClip TargetFeedbackSound;
    private AudioSource audioSource;

    private Material[] defaultMaterials;
    private bool placing = false;

	// Use this for initialization
	void Start () {
        defaultMaterials = GetComponent<Renderer>().materials;

        //Add collider if this object doesnt have one.
        Collider collider = GetComponentInChildren<Collider>();
        if(collider == null)
        {
            gameObject.AddComponent<BoxCollider>();
        }

        //MS tutorial suggests having audio/haptic feedback
	}
	
	// Update is called once per frame
	void Update () {
	}

    void GazeEntered()
    {
        for(int i = 0; i < defaultMaterials.Length; i++)
        {
            defaultMaterials[i].SetFloat("_Highlight", .25f);
        }
        //SpatialMappingManager.Instance.DrawVisualMeshes = true;
    }

    void GazeExited()
    {
        for(int i = 0; i < defaultMaterials.Length; i++)
        {
            defaultMaterials[i].SetFloat("_Highlight", .0f);
        }
        SpatialMappingManager.Instance.DrawVisualMeshes = false;
    }

    void OnSelect()
    {
        for (int i = 0; i < defaultMaterials.Length; i++)
        {
            defaultMaterials[i].SetFloat("_Highlight", .5f);
        }
        /*//SpatialMappingManager.Instance.DrawVisualMeshes = true;
        placing = !placing;
        //If an object is selected, show world mesh.
        if (placing)
        {
            SpatialMappingManager.Instance.DrawVisualMeshes = true;

            var headPos = Camera.main.transform.position;
            var headDir = Camera.main.transform.forward;

            RaycastHit hitInfo;
            bool hit = Physics.Raycast(headPos, headDir, out hitInfo, 30.0f, SpatialMappingManager.Instance.PhysicsLayer);

            if (hit)
            {
                this.transform.position = hitInfo.point;

            }

        }
        //Object not selected, hide world mesh
        else
        {
            SpatialMappingManager.Instance.DrawVisualMeshes = false;
        }*/
        Painter.Instance.PaintObject();
    }
}
