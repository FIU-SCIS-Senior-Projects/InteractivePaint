#include "BrushModeSelectorMenu.h"

namespace touchpoints { namespace ui {
	BrushModeSelectorMenu::BrushModeSelectorMenu() {}

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible,
		drawing::Brush* brush, function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler)
		: Menu(startPoint, width, height, visible), brush(brush), touchEventHandler(touchEventHandler) { }

	BrushModeSelectorMenu::BrushModeSelectorMenu(vec2 startPoint, int width, int height, bool visible,
		drawing::Brush* brush, function<void(vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)> touchEventHandler,
	    multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: Menu(startPoint, width, height, visible, composingShapes), brush(brush), 
		touchEventHandler(touchEventHandler) { }

	void BrushModeSelectorMenu::OnTouch(vec2 point)
	{
		if (visible && boundingRect.Contains(point))
		{
			touchEventHandler(point, this, brush);
		}
	}
}}
