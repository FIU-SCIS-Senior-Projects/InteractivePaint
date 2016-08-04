using UnityEngine;
using UnityEngine.UI;

namespace HoloToolkit.Unity
{
    /// <summary>
    /// Simple Behaviour which calculates the frames per seconds and shows the FPS in a referenced Text control.
    /// </summary>
    public class LocDisplay : MonoBehaviour
    {
        [Tooltip("Reference to Text UI control where the Loc should be displayed.")]
        public Text Text;

        //private float _deltaTime;

        private void Update()
        {
            //_deltaTime += (Time.deltaTime - _deltaTime) * 0.1f;
            //var msec = _deltaTime * 1000.0f;
            //var fps = 1.0f / _deltaTime;
            //var text = GazeManager.Instance.Position.ToString("G4");
            var text = VoiceCmdManager.Instance.prevCommand;
            Text.text = text;
            // Debug.Log(text);
        }
    }
}