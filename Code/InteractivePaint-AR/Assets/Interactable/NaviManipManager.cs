using System;
using HoloToolkit.Unity;
using UnityEngine;
using UnityEngine.VR.WSA.Input;

public class NaviManipManager : Singleton<NaviManipManager> {

    public GestureRecognizer NavigationRecog { get; private set; }
    public GestureRecognizer ManipulationRecog { get; private set; }
    //ActiveRecog switches between nav and manip
    //Whichever one is 'active', only one can be used at a time since they share the same gesture
    //but actions are vastly different
    public GestureRecognizer ActiveRecog { get; private set; }

    public Vector3 NavigationPos { get; private set; }
    public Vector3 ManipulationPos { get; private set; }

    public bool isNavigation { get; private set; }
    public bool isManipulation { get; private set; }

	// Use this for initialization
	//void Start () {
	
	//}

    void Awake() {
        NavigationRecog = new GestureRecognizer();
        ManipulationRecog = new GestureRecognizer();

        Init_NavigationRecog(NavigationRecog);
        Init_ManipulationRecog(ManipulationRecog);
    }
	
	// Update is called once per frame
	void Update () {
	
	}
    /*Initalize Recognizers*/
    void Init_NavigationRecog(GestureRecognizer nav)
    {
        nav.SetRecognizableGestures(GestureSettings.Tap);
        nav.TappedEvent += Nav_Tapped;
        nav.NavigationStartedEvent += Nav_Started;
        nav.NavigationCompletedEvent += Nav_Completed;
        nav.NavigationUpdatedEvent += Nav_Updated;
        
    }

    void Init_ManipulationRecog(GestureRecognizer manip)
    {
        manip.SetRecognizableGestures(GestureSettings.ManipulationTranslate);
        manip.ManipulationStartedEvent += Manip_Started;
        manip.ManipulationUpdatedEvent += Manip_Updated;
        manip.ManipulationCompletedEvent += Manip_Completed;
        manip.ManipulationCanceledEvent += Manip_Cancelled;
    }

    private void Manip_Cancelled(InteractionSourceKind src, Vector3 pos, Ray ray)
    {
        isManipulation = false;
    }

    private void Manip_Completed(InteractionSourceKind src, Vector3 pos, Ray ray)
    {
        isManipulation = false;
    }

    private void Manip_Updated(InteractionSourceKind src, Vector3 pos, Ray ray)
    {
        isManipulation = true;
  
        ManipulationPos = pos;
    }

    private void Manip_Started(InteractionSourceKind src, Vector3 pos, Ray ray)
    {
        throw new NotImplementedException();
    }

    /* Navigation events below */
    private void Nav_Tapped(InteractionSourceKind src, int tapCount, Ray ray)
    {
        GameObject focusedObject = GazeManager.Instance.HitInfo.collider.gameObject;

        if (focusedObject != null)
        {
            focusedObject.SendMessage("OnSelect");
        }
    }

    private void Nav_Started(InteractionSourceKind src, Vector3 pos, Ray ray)
    {
        isNavigation = true;
        NavigationPos = pos;
    }

    private void Nav_Completed(InteractionSourceKind src, Vector3 pos, Ray ray) 
    {
        isNavigation = false;
    }

    private void Nav_Updated(InteractionSourceKind src, Vector3 pos, Ray ray)
    {
        isNavigation = true;
        NavigationPos = pos;
    }

}
