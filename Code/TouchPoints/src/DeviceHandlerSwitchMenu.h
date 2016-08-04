#pragma once
#include "SwitchMenu.h"
#include "Devices.h"
#include "DeviceHandler.h"

namespace touchpoints { namespace ui
{
	class DeviceHandlerSwitchMenu : public SwitchMenu
	{
	public:
		DeviceHandlerSwitchMenu();
		DeviceHandlerSwitchMenu(vec2 startPoint, devices::Devices device, devices::DeviceHandler* deviceHandler,
			bool visible = false, int width = defaultWidth, int height = defaultHeight, ColorA onColor = Menu::green,
			ColorA offColor = Menu::black);
		void OnTouch(vec2 point) override;
		void Draw() override;
		static const int defaultHeight;
		static const int defaultWidth;
	private:
		devices::Devices device;
		devices::DeviceHandler* deviceHandler;
	};
}}
