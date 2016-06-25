#pragma once
#include "Kinect.h"
#include "stdafx.h"
#include "Illustrator.h"
#include "GuidHash.h"


namespace touchpoints { namespace devices
{

	class KinectHandler 
	{
	public:
		KinectHandler();

		D2D1_POINT_2F BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height);
		HRESULT InitializeDefaultSensor();
		void ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies, drawing::Illustrator& illustrator); // Addedd drawing 
		void Update(drawing::Illustrator& illustrator); //Added drawing

	private:
		HWND                    m_hWnd;
		INT64                   m_nStartTime;
		INT64                   m_nLastCounter;
		double                  m_fFreq;
		INT64                   m_nNextStatusTime;
		DWORD                   m_nFramesSinceUpdate;
		
		GuidGenerator			guidGenerator;
		Guid getGuid();

		drawing::DrawEvent finalizeableLeftDrawEvents;
		drawing::DrawEvent finalizeableRightDrawEvents;
		
		
		drawing::DrawEvent temporaryLeftDrawEvents;
		drawing::DrawEvent temporaryRightDrawEvents;


		

		//D2D1_POINT_2F BodyToScreen(const CameraSpacePoint&, int, int);

		// Current Kinect
		IKinectSensor*          m_pKinectSensor;
		ICoordinateMapper*      m_pCoordinateMapper;

		// Body reader
		IBodyFrameReader*       m_pBodyFrameReader;

		static const int        cDepthWidth = 512;
		static const int        cDepthHeight = 424;



		// Body/hand drawing
		ID2D1HwndRenderTarget*  m_pRenderTarget;
		ID2D1SolidColorBrush*   m_pBrushJointTracked;
		ID2D1SolidColorBrush*   m_pBrushJointInferred;
		ID2D1SolidColorBrush*   m_pBrushBoneTracked;
		ID2D1SolidColorBrush*   m_pBrushBoneInferred;
		ID2D1SolidColorBrush*   m_pBrushHandClosed;
		ID2D1SolidColorBrush*   m_pBrushHandOpen;
		ID2D1SolidColorBrush*   m_pBrushHandLasso;
	}

	;
}
}