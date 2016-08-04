#pragma once
#include "Menu.h"
#include "Brush.h"

namespace touchpoints { namespace ui {
	class BrushModeSelectorMenu : public Menu
	{
	public:
		BrushModeSelectorMenu();
		BrushModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler);
		BrushModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		BrushModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
			multimap<int, shared_ptr<Menu>> composingMenus);
		BrushModeSelectorMenu(vec2 startPoint, bool visible, ColorA backgroundColor, string imagePath,
			drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler);
		BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler);
		BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush,
			function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		virtual bool OnTouch(vec2 point) override;
	protected:
		drawing::Brush* brush;
		function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler;
	};
}}
