#include "BrushModeSelectorMenu.h"

namespace touchpoints { namespace ui {
	BrushModeSelectorMenu::BrushModeSelectorMenu() {}

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
		function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler)
		: Menu(startPoint, Menu::defaultWidth, Menu::defaultHeight, visible, nullptr), brush(brush),
		touchEventHandler(touchEventHandler) { }

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
		function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: Menu(startPoint, Menu::defaultWidth, Menu::defaultHeight, visible, composingShapes, nullptr),
		brush(brush), touchEventHandler(touchEventHandler) { }

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
		function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
		multimap<int, shared_ptr<Menu>> composingMenus)
		: Menu(startPoint, Menu::defaultWidth, Menu::defaultHeight, visible, composingMenus, nullptr),
		brush(brush), touchEventHandler(touchEventHandler) { }

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, bool visible, ColorA backgroundColor, string imagePath,
		drawing::Brush* brush,
		function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler)
		: Menu(startPoint, visible, backgroundColor, imagePath, nullptr), brush(brush),
		touchEventHandler(touchEventHandler) { }

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible,
		drawing::Brush* brush, function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler)
		: Menu(startPoint, width, height, visible, nullptr), brush(brush), touchEventHandler(touchEventHandler) { }

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible,
		drawing::Brush* brush, function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
	    multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: Menu(startPoint, width, height, visible, composingShapes, nullptr), brush(brush),
		touchEventHandler(touchEventHandler) { }

	bool BrushModeSelectorMenu::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		if (touchEventHandler != nullptr && visible && boundingRect.Contains(point))
		{
			touchWasHandled = true;
			touchEventHandler(point, this, brush);
		}

		Menu::OnTouch(point);
		return touchWasHandled;
	}
}}
