#pragma once
#include "Menu.h"
#include "Brush.h"

namespace touchpoints { namespace ui {
	class BrushModeSelectorMenu : public Menu
	{
	public:
		BrushModeSelectorMenu();
		BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler);
		BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		void OnTouch(vec2 point) override;
	private:
		drawing::Brush* brush;
		function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler;
	};
}}
