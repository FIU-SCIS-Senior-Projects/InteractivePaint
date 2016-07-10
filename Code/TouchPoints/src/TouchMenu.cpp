#include "TouchMenu.h"

namespace touchpoints { namespace ui {

	TouchMenu::TouchMenu() {}

	TouchMenu::TouchMenu(vec2 startPoint, int width, int height, bool visible, 
		function<void(vec2 point, TouchMenu *self)> touchEventHandler)
		: Menu(startPoint, width, height, visible), touchEventHandler(touchEventHandler) { }

	TouchMenu::TouchMenu(vec2 startPoint, int width, int height, bool visible, 
		function<void(vec2 point, TouchMenu *self)> touchEventHandler,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: Menu(startPoint, width, height, visible, composingShapes), touchEventHandler(touchEventHandler) { }

	void TouchMenu::OnTouch(vec2 point)
	{
		if (visible && boundingRect.Contains(point))
		{
			touchEventHandler(point, this);
		}
	}
}}