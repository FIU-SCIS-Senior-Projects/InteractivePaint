using UnityEngine;
using UnityEngine.VR.WSA.Input;
using HoloToolkit.Unity;
using System.Collections;
using System;
    public partial class GestureGazeManager : Singleton<GestureGazeManager> {

        //public static GestureGazeManager Instance { get; private set; }

        [Tooltip("Maximum gaze distance that will be used to calculate a hit.")]
        public float MaxGazeDistance;

        [Tooltip("Layers raycast will target")]
        public LayerMask RayCastLayerMask;

        public GameObject FocusedObject { get; private set; }//object hit with raycast
        public RaycastHit HitInfo { get; private set; }//raycast info if we hit
        public Vector3 HeadPos { get; private set; }//head position in space
        public Vector3 HeadDir { get; private set; }//head facing direction
        public Vector3 Position { get; private set; }
        public Vector3 Normal { get; private set; }

        public bool Hit { get; private set; }

        private Vector3 headPos;
        private Vector3 headDir;
        private int layermask;//to ignore specific layers with raycast (namely layer 31 : spatial mapping mesh)

        GestureRecognizer recognizer;
	    // Use this for initialization
	    void Start () {
            //Instance = this;

            //recognizer = new GestureRecognizer();
            ////add events to the recognizer, also lambda expression
            ////setup to detect only select gestures
            //recognizer.TappedEvent += (source, taps, ray) =>
            //{
            //    if(FocusedObject != null)
            //    {
            //        //send message up ancestors list
            //        //e.g. if we select our 'paint', send the message to the canvas too so we can 'select' that too
            //        FocusedObject.SendMessageUpwards("OnSelect");
            //    }
            //};
            //recognizer.StartCapturingGestures();
	    }
	
	    // Update is called once per frame
	    void Update () {
            //get the current object in gaze
            GameObject oldObjectFocus = FocusedObject;

            headPos = Camera.main.transform.position;//get head position in space
            headDir = Camera.main.transform.forward;//get head facing direction

            //Do a raycast into space based off headpos/dir
            RaycastHit hitInfo;//cannot pass object into the raycast, so need this
            Hit = Physics.Raycast(headPos, headDir, out hitInfo, MaxGazeDistance);
            //update public property so others can access
            HitInfo = hitInfo;


            //Did we hit an object with the ray?
            if(Hit)
            {
                //yes we did, get that object
                Position = hitInfo.point;
                Normal = hitInfo.normal;

                FocusedObject = hitInfo.collider.gameObject;
            
            }
            else
            {
                //no we didnt, clear focus
                FocusedObject = null;
                Position = headPos + (headDir * hitInfo.distance);
                Normal = headDir;
            
            }
            //Is this object something new?
            if(FocusedObject != oldObjectFocus)
            {
                //recognizer.CancelGestures();//stop reading gestures on previous object
                //recognizer.StartCapturingGestures();//read them on the new one
            }
	    }
    }
