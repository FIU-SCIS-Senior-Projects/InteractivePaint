#pragma once
#include "Menu.h"

namespace touchpoints { namespace ui
{
	class ModeSelectorMenu : public Menu
	{
	public:
		ModeSelectorMenu(); 
		ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
			function<void(vec2 point, Menu *self)> touchEventHandler);
		ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
			function<void(vec2 point, Menu *self)> touchEventHandler);
		ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
			multimap<int, shared_ptr<Menu>> composingMenus,
			function<void(vec2 point, Menu *self)> touchEventHandler);
		ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
			multimap<int, shared_ptr<Menu>> composingMenus,
			function<void(vec2 point, Menu *self)> touchEventHandler);
	private:
		string filePath;
		void populateShapes();
	};
}}