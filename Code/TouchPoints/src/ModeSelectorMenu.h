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
		static const int lineThickness = 5;
		static const ColorA grey;
		static const ColorA purple;
		static const int width = 60;
		static const int height = 60;
		static const int imageWidth = 50;
		static const int imageHeight = 50;
		static const int imageOffsetX = 5;
		static const int imageOffsetY = 5;
	private:
		string filePath;
		void populateShapes();
		//function<void(vec2 point, ModeSelectorMenu *self)> touchEventHandler;
	};
}}