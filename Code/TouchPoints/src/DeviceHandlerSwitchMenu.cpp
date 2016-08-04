#include "DeviceHandlerSwitchMenu.h"

namespace touchpoints { namespace ui
{
	const int DeviceHandlerSwitchMenu::defaultHeight = 32;
	const int DeviceHandlerSwitchMenu::defaultWidth = 32;

	DeviceHandlerSwitchMenu::DeviceHandlerSwitchMenu() { }

	DeviceHandlerSwitchMenu::DeviceHandlerSwitchMenu(vec2 startPoint, devices::Devices device, 
		devices::DeviceHandler* deviceHandler, bool visible, int width, int height,
		ColorA onColor, ColorA offColor) 
		: SwitchMenu(startPoint, width, height, visible, false, onColor, offColor),
		device(device), deviceHandler(deviceHandler) { }

	void DeviceHandlerSwitchMenu::OnTouch(vec2 point)
	{
		switch(device)
		{
			case devices::MultiTouch :
			{
				deviceHandler->toggleMultiTouch();
				break;
			}
			case devices::EyeX :
			{
				deviceHandler->toggleEyeX();
				break;
			}
			case devices::LeapMotion :
			{
				deviceHandler->toggleLeap();
				break;
			}
			case devices::LeapDraw :
			{
				deviceHandler->toggleLeapDraw();
				break;
			}
			case devices::LeapGesture :
			{
				deviceHandler->toggleLeapGesture();
				break;
			}
			case devices::RealSense :
			{
				deviceHandler->toggleRealSense();
				break;
			}
			case devices::RealSenseDraw :
			{
				deviceHandler->toggleRealSenseDraw();
				break;
			}
			case devices::RealSenseExpressions :
			{
				deviceHandler->toggleRealSenseExpressions();
				break;
			}
		}
	}

	void DeviceHandlerSwitchMenu::Draw()
	{
		if(!visible)
		{
			return;
		}

		bool shouldBeOn = false;

		switch (device)
		{
			case devices::MultiTouch:
			{
				shouldBeOn = deviceHandler->multiTouchStatus();
				break;
			}
			case devices::EyeX:
			{
				shouldBeOn = deviceHandler->eyeXStatus();
				break;
			}
			case devices::LeapMotion:
			{
				shouldBeOn = deviceHandler->leapStatus();
				break;
			}
			case devices::LeapDraw:
			{
				shouldBeOn = deviceHandler->leapDrawStatus();
				break;
			}
			case devices::LeapGesture:
			{
				shouldBeOn = deviceHandler->leapGestureStatus();
				break;
			}
			case devices::RealSense:
			{
				shouldBeOn = deviceHandler->realSenseStatus();
				break;
			}
			case devices::RealSenseDraw:
			{
				shouldBeOn = deviceHandler->realSenseDrawStatus();
				break;
			}
			case devices::RealSenseExpressions:
			{
				shouldBeOn = deviceHandler->realSenseExpressionsStatus();
				break;
			}
		}
		auto currentColor = shouldBeOn ? onColor : offColor;

		rect.setColor(currentColor);
		rect.Draw();
		greyBorder.Draw();
	}
}}