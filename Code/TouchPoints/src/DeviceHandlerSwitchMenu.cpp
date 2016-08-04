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

	bool DeviceHandlerSwitchMenu::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		switch(device)
		{
			case devices::MultiTouch :
			{
				touchWasHandled = true;
				deviceHandler->toggleMultiTouch();
				break;
			}
			case devices::EyeX :
			{
				touchWasHandled = true;
				deviceHandler->toggleEyeX();
				break;
			}
			case devices::LeapMotion :
			{
				touchWasHandled = true;
				deviceHandler->toggleLeap();
				break;
			}
			case devices::LeapDraw :
			{
				touchWasHandled = true;
				deviceHandler->toggleLeapDraw();
				break;
			}
			case devices::LeapGesture :
			{
				touchWasHandled = true;
				deviceHandler->toggleLeapGesture();
				break;
			}
			case devices::RealSense :
			{
				touchWasHandled = true;
				deviceHandler->toggleRealSense();
				break;
			}
			case devices::RealSenseDraw :
			{
				touchWasHandled = true;
				deviceHandler->toggleRealSenseDraw();
				break;
			}
			case devices::RealSenseExpressions :
			{
				touchWasHandled = true;
				deviceHandler->toggleRealSenseExpressions();
				break;
			}
		}
		return touchWasHandled;
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