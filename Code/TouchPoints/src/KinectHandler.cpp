#include "KinectHandler.h"

namespace touchpoints {namespace devices
{
	KinectHandler::KinectHandler()
	{
		InitializeDefaultSensor();
	}

	/*
	This converts given Kinect Camera point to a point within a certain range.
	Of width*height?
	*/

	D2D1_POINT_2F KinectHandler::BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height)
	{
		// Calculate the body's position on the screen
		DepthSpacePoint depthPoint = {0};
		m_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

		float screenPointX = static_cast<float>(depthPoint.X * width) / cDepthWidth;
		float screenPointY = static_cast<float>(depthPoint.Y * height) / cDepthHeight;

		return D2D1::Point2F(screenPointX, screenPointY);
	}

	/*
	bool SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce)
	{
	INT64 now = GetTickCount64();

	if (m_hWnd && (bForce || (m_nNextStatusTime <= now)))
	{
	//SetDlgItemText(m_hWnd, IDC_STATUS, szMessage);
	m_nNextStatusTime = now + nShowTimeMsec;

	return true;
	}

	return false;
	}*/

	/* Initializes the Kinect */
	HRESULT KinectHandler::InitializeDefaultSensor()
	{
		HRESULT hr;

		hr = GetDefaultKinectSensor(&m_pKinectSensor);
		if (FAILED(hr))
		{
			return hr;
		}

		if (m_pKinectSensor)
		{
			// Initialize the Kinect and get coordinate mapper and the body reader
			IBodyFrameSource* pBodyFrameSource = NULL;

			hr = m_pKinectSensor->Open();

			if (SUCCEEDED(hr))
			{
				hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
			}

			if (SUCCEEDED(hr))
			{
				hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
			}

			SafeRelease(pBodyFrameSource);
		}

		if (!m_pKinectSensor || FAILED(hr))
		{
			//SetStatusMessage(L"No ready Kinect found!", 10000, true);
			//cout << "Kinect not connected.\n";
			return E_FAIL;
		}

		return hr;
	}

	/*Another function

	*/
	void KinectHandler::ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies, drawing::Illustrator& illustrator)
	{
		int width = 1500;
		int height = 1000;
		//HRESULT hr = EnsureDirect2DResources();
		HRESULT hr;
		/*if (SUCCEEDED(hr) && m_pRenderTarget && m_pCoordinateMapper)
		{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->Clear();

		/*RECT rct;
		GetClientRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rct);
		int width = rct.right;
		int height = rct.bottom;
		*/
		for (int i = 0; i < nBodyCount; ++i)
		{
			IBody* pBody = ppBodies[i];
			if (pBody)
			{
				BOOLEAN bTracked = false;
				hr = pBody->get_IsTracked(&bTracked);

				if (SUCCEEDED(hr) && bTracked)
				{
					Joint joints[JointType_Count];
					D2D1_POINT_2F jointPoints[JointType_Count];
					HandState leftHandState = HandState_Unknown;
					HandState rightHandState = HandState_Unknown;

					pBody->get_HandLeftState(&leftHandState);
					pBody->get_HandRightState(&rightHandState);

					hr = pBody->GetJoints(_countof(joints), joints);
					if (SUCCEEDED(hr))
					{
						for (int j = 0; j < _countof(joints); ++j)
						{
							jointPoints[j] = BodyToScreen(joints[j].Position, width, height);
						}

						/*
						For testing results.  With BodyToScreen converting the position points from joints array and storing in jointPoints array,
						One can retreive position information about any JointType enumerations.
						*/
						/*
						myFile.open("example.txt", fstream::app);
						myFile << "With bodytoscreen:" << jointPoints[JointType_HandLeft].x << "," << jointPoints[JointType_HandLeft].y << "\n";
						myFile.close();
						*/
						auto limeGreen = ColorA(0.33f, 1.0f, 0.0f, 1);

						float HandLeftXCoordinate = jointPoints[JointType_HandLeft].x;
						float HandLeftYCoordinate = jointPoints[JointType_HandLeft].y;

						auto currentLeftHandPoint = vec2(HandLeftXCoordinate, HandLeftYCoordinate);

						float HandRightXCoordinate = jointPoints[JointType_HandRight].x;
						float HandRightYCoordinate = jointPoints[JointType_HandRight].y;

						auto currentRightHandPoint = vec2(HandRightXCoordinate, HandRightYCoordinate);

						auto HandLocationCircles = vector<drawing::TouchCircle>();
						auto handLocationRate = 20;

						vector<drawing::DrawEvent> sendToIllustrator;

						if (leftHandState == HandState_Open)
						{
							if (finalizeableLeftDrawEvents.HasStartPoint())
							{
								finalizeableLeftDrawEvents.SetEndPoint(currentLeftHandPoint);

								sendToIllustrator.push_back(finalizeableLeftDrawEvents);

								finalizeableLeftDrawEvents = drawing::DrawEvent();
							}

							HandLocationCircles.push_back(drawing::TouchCircle(currentLeftHandPoint, 40.0f, limeGreen, 50.0f, true, handLocationRate));
						}
						else if (leftHandState == HandState_Closed)
						{
							if (finalizeableLeftDrawEvents.HasStartPoint())
							{
								if (temporaryLeftDrawEvents.HasStartPoint()) //continuation of series of draw events
								{
									temporaryLeftDrawEvents.SetEndPoint(currentLeftHandPoint);

									sendToIllustrator.push_back(temporaryLeftDrawEvents);

									temporaryLeftDrawEvents = drawing::DrawEvent();
								}
								else
								{
									auto guid = getGuid();
									temporaryLeftDrawEvents = drawing::DrawEvent(currentLeftHandPoint, finalizeableLeftDrawEvents.GetStartPoint(), guid, false, 0);
								}
							}
							else
							{
								auto guid = getGuid();

								finalizeableLeftDrawEvents = drawing::DrawEvent(currentLeftHandPoint, guid, true, 0);
							}
						}

						if (rightHandState == HandState_Open)
						{
							if (finalizeableRightDrawEvents.HasStartPoint())
							{
								finalizeableRightDrawEvents.SetEndPoint(currentRightHandPoint);

								sendToIllustrator.push_back(finalizeableRightDrawEvents);

								finalizeableRightDrawEvents = drawing::DrawEvent();
							}

							HandLocationCircles.push_back(drawing::TouchCircle(currentRightHandPoint, 40.0f, limeGreen, 50.0f, true, handLocationRate));
						}
						else if (rightHandState == HandState_Closed)
						{
							if (finalizeableRightDrawEvents.HasStartPoint())
							{
								if (temporaryRightDrawEvents.HasStartPoint()) //continuation of series of draw events
								{
									temporaryRightDrawEvents.SetEndPoint(currentRightHandPoint);

									sendToIllustrator.push_back(temporaryRightDrawEvents);

									temporaryRightDrawEvents = drawing::DrawEvent();
								}
								else
								{
									auto guid = getGuid();
									temporaryRightDrawEvents = drawing::DrawEvent(currentRightHandPoint, finalizeableRightDrawEvents.GetStartPoint(), guid, false, 0);
								}
							}
							else
							{
								auto guid = getGuid();
								finalizeableRightDrawEvents = drawing::DrawEvent(currentRightHandPoint, guid, true, 0);
							}
						}
						if (HandLocationCircles.size() > 0)
						{
							illustrator.addToTemporaryCircles(HandLocationCircles);
						}
						if (sendToIllustrator.size() > 0)
						{
							illustrator.addDrawEventsToQueue(sendToIllustrator);
						}
					}
				}
			}
		}

		//hr = m_pRenderTarget->EndDraw();

		// Device lost, need to recreate the render target
		// We'll dispose it now and retry drawing
		if (D2DERR_RECREATE_TARGET == hr)
		{
			hr = S_OK;
			//DiscardDirect2DResources();
		}

		if (!m_nStartTime)
		{
			m_nStartTime = nTime;
		}

		double fps = 0.0;

		LARGE_INTEGER qpcNow = {0};
		if (m_fFreq)
		{
			if (QueryPerformanceCounter(&qpcNow))
			{
				if (m_nLastCounter)
				{
					m_nFramesSinceUpdate++;
					fps = m_fFreq * m_nFramesSinceUpdate / double(qpcNow.QuadPart - m_nLastCounter);
				}
			}
		}

		//		WCHAR szStatusMessage[64];
		//StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f    Time = %I64d", fps, (nTime - m_nStartTime));

		//if (SetStatusMessage(szStatusMessage, 1000, false))
		//{
		//m_nLastCounter = qpcNow.QuadPart;
		//m_nFramesSinceUpdate = 0;
		//}
	}

	//}

	Guid KinectHandler::getGuid()
	{
		return guidGenerator.newGuid();
	}

	/*

	Another function

	*/

	void KinectHandler::Update(drawing::Illustrator& illustrator)
	{
		if (!m_pBodyFrameReader)
		{
			//cout << "Didn't get anywhere.\n";

			return;
		}

		IBodyFrame* pBodyFrame = NULL;

		HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

		if (SUCCEEDED(hr))
		{
			INT64 nTime = 0;

			hr = pBodyFrame->get_RelativeTime(&nTime);

			IBody* ppBodies[BODY_COUNT] = {0};

			if (SUCCEEDED(hr))
			{
				hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
			}

			if (SUCCEEDED(hr))
			{
				ProcessBody(nTime, BODY_COUNT, ppBodies, illustrator);
				//ProcessColor(nTime, )
			}

			for (int i = 0; i < _countof(ppBodies); ++i)
			{
				SafeRelease(ppBodies[i]);
			}
		}

		SafeRelease(pBodyFrame);
	}
}}
