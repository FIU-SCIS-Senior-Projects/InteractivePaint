#include "SymmetryModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	SymmetryModeSelectorMenu::SymmetryModeSelectorMenu() {}

	SymmetryModeSelectorMenu::SymmetryModeSelectorMenu(vec2 startPoint, int height, 
		int width, bool visible, drawing::SymmetryLine* symmetryLine,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, SymmetryModeSelectorMenu* self,
			drawing::SymmetryLine* symmetryLineLambda)> touchEventHandler) 
		: Menu(startPoint, height, width, visible, composingShapes, composingMenus, nullptr),
		symmetryLine(symmetryLine), touchEventHandler(touchEventHandler) { }

	bool SymmetryModeSelectorMenu::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		if(touchEventHandler != nullptr && visible && boundingRect.Contains(point))
		{
			touchWasHandled = true;
			touchEventHandler(point, this, symmetryLine);
		}
		return touchWasHandled;
	}
}}