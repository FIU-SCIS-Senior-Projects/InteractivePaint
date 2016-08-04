#pragma once
#include "Menu.h"
#include "Devices.h"
#include "DeviceHandler.h"
#include "LabelMenu.h"
#include "DeviceHandlerSwitchMenu.h"

namespace touchpoints { namespace ui 
{
	class DeviceHandlerModeSelectorMenu : public Menu
	{
	public:
		DeviceHandlerModeSelectorMenu();
		DeviceHandlerModeSelectorMenu(vec2 startPoint, bool visible, string labelText, 
			devices::DeviceHandler* deviceHandler, devices::Devices device,
			function<void(vec2 point, DeviceHandlerModeSelectorMenu* self,
				devices::DeviceHandler* deviceHandlerLambda)> touchEventHandler = nullptr,
			int width = DeviceHandlerModeSelectorMenu::defaultWidth, 
			int height = DeviceHandlerModeSelectorMenu::defaultHeight,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes =
			multimap<int, shared_ptr<drawing::TouchShape>>(),
			multimap<int, shared_ptr<Menu>> composingMenus = multimap<int, shared_ptr<Menu>>());
		virtual void OnTouch(vec2 point) override;
		static const int defaultHeight;
	private:
		static const int defaultWidth;
		static const int defaultSwitchWidth;
		devices::Devices device;
		string labelText;
		shared_ptr<DeviceHandlerSwitchMenu> switchMenu;
		devices::DeviceHandler* deviceHandler;
		function<void(vec2 point, DeviceHandlerModeSelectorMenu* self,
			devices::DeviceHandler* deviceHandlerLambda)> touchEventHandler;
		void setupMenus();
	};
}}