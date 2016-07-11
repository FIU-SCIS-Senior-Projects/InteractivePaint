#pragma once
#include "MenuGroup.h"

namespace touchpoints { namespace ui
{
	class ModeSelectorMenu : public MenuGroup
	{
	public:
		ModeSelectorMenu(); 
		ModeSelectorMenu(vec2 startPoint, string filePath, 
			function<void(vec2 point, ModeSelectorMenu *self)> touchEventHandler);
		ModeSelectorMenu(vec2 startPoint, string filePath, 
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
			function<void(vec2 point, ModeSelectorMenu *self)> touchEventHandler);
		ModeSelectorMenu(vec2 startPoint, string filePath,
			multimap<int, shared_ptr<IMenu>> composingMenus,
			function<void(vec2 point, ModeSelectorMenu *self)> touchEventHandler);
		ModeSelectorMenu(vec2 startPoint, string filePath, 
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
			multimap<int, shared_ptr<IMenu>> composingMenus,
			function<void(vec2 point, ModeSelectorMenu *self)> touchEventHandler);
		void OnTouch(vec2 point) override;
		void ToggleContainingMenusVisibility();
		static const int lineThickness = 5;
		static const ColorA grey;
		static const ColorA purple;
	private:
		vec2 startPoint;
		string filePath;
		const int width = 60;
		const int height = 60;
		const int imageWidth = 50;
		const int imageHeight = 50;
		const int imageOffsetX = 5;
		const int imageOffsetY = 5;
		void populateShapes();
		function<void(vec2 point, ModeSelectorMenu *self)> touchEventHandler;
	};
}}