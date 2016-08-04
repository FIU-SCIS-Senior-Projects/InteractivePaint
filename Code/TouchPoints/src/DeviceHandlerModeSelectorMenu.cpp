#include "DeviceHandlerModeSelectorMenu.h"


namespace touchpoints { namespace ui
{
	const int DeviceHandlerModeSelectorMenu::defaultHeight = 32;
	const int DeviceHandlerModeSelectorMenu::defaultWidth = 205;
	const int DeviceHandlerModeSelectorMenu::defaultSwitchWidth = 32;

	DeviceHandlerModeSelectorMenu::DeviceHandlerModeSelectorMenu() { }
	
	DeviceHandlerModeSelectorMenu::DeviceHandlerModeSelectorMenu(vec2 startPoint, bool visible,
		string labelText, devices::DeviceHandler* deviceHandler, devices::Devices device,
		function<void(vec2 point, DeviceHandlerModeSelectorMenu* self,
			devices::DeviceHandler* deviceHandlerLambda)> touchEventHandler, 
		int width, int height,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		multimap<int, shared_ptr<Menu>> composingMenus) 
		: Menu(startPoint, width, height, visible, composingShapes, composingMenus, nullptr),
		labelText(labelText), deviceHandler(deviceHandler), touchEventHandler(touchEventHandler),
		device(device)
	{
		setupMenus();
	}
	
	void DeviceHandlerModeSelectorMenu::OnTouch(vec2 point)
	{
		if (visible && boundingRect.Contains(point))
		{
			switchMenu->OnTouch(point);
		}
	}

	void DeviceHandlerModeSelectorMenu::setupMenus()
	{
		switchMenu = shared_ptr<DeviceHandlerSwitchMenu>(new DeviceHandlerSwitchMenu(startPoint, device,
			deviceHandler, visible));
		composingMenus.insert(make_pair(0, switchMenu));

		auto labelX = startPoint.x + defaultSwitchWidth;
		auto labelWidth = DeviceHandlerModeSelectorMenu::defaultWidth - defaultSwitchWidth;
		auto labelStartPoint = vec2(labelX, startPoint.y);
		auto labelMenu = shared_ptr<LabelMenu>(new LabelMenu(labelStartPoint, labelWidth, defaultHeight,
			visible, labelText, 50));
		composingMenus.insert(make_pair(0, labelMenu));
	}
}}
