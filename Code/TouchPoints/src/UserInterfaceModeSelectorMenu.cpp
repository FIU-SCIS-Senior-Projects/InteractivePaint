#include "UserInterfaceModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	UserInterfaceModeSelectorMenu::UserInterfaceModeSelectorMenu() { }
	
	UserInterfaceModeSelectorMenu::UserInterfaceModeSelectorMenu(vec2 startPoint, int width, int height,
		bool visible, shared_ptr<UserInterface> ui, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		function<void(vec2 point, UserInterfaceModeSelectorMenu* self, shared_ptr<UserInterface> uiLambda)> touchEventHandler)
		: Menu(startPoint, width, height, visible, composingShapes, nullptr), ui(ui), 
		touchEventHandler(touchEventHandler) { }
	
	bool UserInterfaceModeSelectorMenu::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		if (visible && touchEventHandler != nullptr && boundingRect.Contains(point))
		{
			touchWasHandled = true;
			touchEventHandler(point, this, ui);
		}
		return touchWasHandled;
	}
}}