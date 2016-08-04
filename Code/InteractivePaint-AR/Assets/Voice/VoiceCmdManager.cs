using UnityEngine;
using UnityEngine.Windows.Speech;
using System.Collections.Generic;
using System.Linq;
using HoloToolkit.Unity;

public class VoiceCmdManager : Singleton<VoiceCmdManager> {
    KeywordRecognizer KeywordRecog;
    Dictionary<string, System.Action> voiceCommands = new Dictionary<string, System.Action>();

    public string prevCommand = "none";
    
    // Use this for initialization
    void Start () {
        voiceCommands.Add("Colors", () => 
        {
            GameObject obj = GameObject.Find("MainImage");
            obj.GetComponent<MeshRenderer>().enabled = true;
            prevCommand = "Colors";
        });

        voiceCommands.Add("Hide Colors", () =>
        {
            GameObject obj = GameObject.Find("MainImage");
            obj.GetComponent<MeshRenderer>().enabled = false;
            prevCommand = "Hide Colors";
        });

        voiceCommands.Add("Add Objects", () =>
        {
            //show menu for adding a new canvas in the world
            prevCommand = "Add Objects";
        });

        voiceCommands.Add("Add Canvas", () =>
        {
            //show menu for adding a new canvas in the world
            Object.Instantiate(GameObject.Find("Canvas"));
            prevCommand = "Add Canvas";
        });

        voiceCommands.Add("Move", () =>
        {
            //enable movement for gazed object
            prevCommand = "Move";
        });

        voiceCommands.Add("Destroy", () =>
        {
            //remove gazed object
            //add protection to prevent colorpicker being removed
            MeshRenderer r = GazeManager.Instance.HitInfo.collider.gameObject.GetComponent<MeshRenderer>();
            r.enabled = !r.enabled;
            prevCommand = "Destroy";
        });

        voiceCommands.Add("Dismiss", () =>
        {
            //should disable the renderer for hi-lighted object
            //MeshRenderer r = GestureGazeManager.Instance.FocusedObject.GetComponent<MeshRenderer>();
            //r.enabled = false;
            prevCommand = "Dismiss";
        });
        
        voiceCommands.Add("Spray", () =>
        {
            Painter.Instance.BrushType = Painter.PaintType.SPRAY;
        });

        KeywordRecog = new KeywordRecognizer(voiceCommands.Keys.ToArray());
        KeywordRecog.OnPhraseRecognized += KeywordRecognizer_OnPhraseRecognized;
        KeywordRecog.Start();

    }
	
	// Update is called once per frame
	void Update () {
	
	}

    private void KeywordRecognizer_OnPhraseRecognized(PhraseRecognizedEventArgs args)
    {
        System.Action keywordAction;
        if (voiceCommands.TryGetValue(args.text, out keywordAction))
        {
            keywordAction.Invoke();
        }
    }
}
