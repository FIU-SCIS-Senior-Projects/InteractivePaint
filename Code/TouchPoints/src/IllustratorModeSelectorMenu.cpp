#include "IllustratorModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	IllustratorModeSelectorMenu::IllustratorModeSelectorMenu() {}

	IllustratorModeSelectorMenu::IllustratorModeSelectorMenu(vec2 startPoint, int height, int width, bool visible,
		drawing::Illustrator* illustrator,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, IllustratorModeSelectorMenu* self,
			drawing::Illustrator* illustratorLambda)> touchEventHandler) 
		: Menu(startPoint, height, width, visible, composingShapes, composingMenus, nullptr),
		illustrator(illustrator), touchEventHandler(touchEventHandler) { }

	bool IllustratorModeSelectorMenu::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		if (touchEventHandler != nullptr && visible && boundingRect.Contains(point))
		{
			touchWasHandled = true;
			touchEventHandler(point, this, illustrator);
		}
		return touchWasHandled;
	}
}}