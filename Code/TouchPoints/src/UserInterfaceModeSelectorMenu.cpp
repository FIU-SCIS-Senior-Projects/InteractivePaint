#include "UserInterfaceModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	UserInterfaceModeSelectorMenu::UserInterfaceModeSelectorMenu() { }
	
	UserInterfaceModeSelectorMenu::UserInterfaceModeSelectorMenu(vec2 startPoint, int width, int height,
		bool visible, shared_ptr<UserInterface> ui, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		function<void(vec2 point, UserInterfaceModeSelectorMenu* self, shared_ptr<UserInterface> uiLambda)> touchEventHandler)
		: Menu(startPoint, width, height, visible, composingShapes, nullptr), ui(ui), 
		touchEventHandler(touchEventHandler) { }
	
	void UserInterfaceModeSelectorMenu::OnTouch(vec2 point)
	{
		if (visible && touchEventHandler != nullptr && boundingRect.Contains(point))
		{
			touchEventHandler(point, this, ui);
		}
	}
}}