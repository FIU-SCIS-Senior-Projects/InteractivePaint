using UnityEngine;
using HoloToolkit.Unity;
using System.Collections;



public class Painter : Singleton<Painter>
{

    public enum PaintType { SPRAY, LINE, BUCKET };
    public PaintType BrushType;
    public GameObject Brush;
    [Tooltip("Texture to use for spray brush")]
    public GameObject SprayBrush;
    [Tooltip("Texture to use for line brush")]
    public GameObject LineBrush;
    [Tooltip("Target render texture to paint on")]
    public RenderTexture CanvasRenderTexture;
    [Tooltip("Camera that looks at the canvas - for rendering")]
    public Camera CanvasCamera;

    Color paintColor;
    float paintSize = 1.0f;



    // Use this for initialization
    void Start()
    {
         
    }

    // Update is called once per frame
    void Update()
    {
        paintColor = ColorPicker.GetColor();
        //manip gesture should be active
        //if manip -> paint where cursor is at

    }

    void WorldToUVPosition(ref Vector3 UVPosition)
    {
        RaycastHit hit = GazeManager.Instance.HitInfo;
        Vector2 pxUV = new Vector2(hit.textureCoord.x, hit.textureCoord.y);

        UVPosition.x = pxUV.x - CanvasCamera.orthographicSize;
        UVPosition.y = pxUV.y - CanvasCamera.orthographicSize;
        UVPosition.z = 0.0f;

    }

    public void PaintObject()
    {
        Vector3 UVPosition = Vector3.zero;
        GameObject brush = null;

        WorldToUVPosition(ref UVPosition);
        brush = GameObject.Instantiate(LineBrush);
        brush.GetComponent<SpriteRenderer>().color = paintColor;
        brush.GetComponent<Material>().color = paintColor;

        switch (BrushType)
        {
            case PaintType.SPRAY:
                brush = GameObject.Instantiate(LineBrush);
                brush.GetComponent<SpriteRenderer>().color = paintColor;
                break;
            case PaintType.BUCKET:
                break;
            case PaintType.LINE:
                break;
        }
        paintColor.a = paintSize * 2.0f;
        brush.transform.parent = GazeManager.Instance.HitInfo.transform;
        brush.transform.localPosition = UVPosition;
        brush.transform.localScale = Vector3.one * paintSize;
    }
}
