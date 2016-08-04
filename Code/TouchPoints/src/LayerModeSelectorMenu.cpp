#include "LayerModeSelectorMenu.h"
#include "Menu.h"

namespace touchpoints {
	namespace ui
	{
		LayerModeSelectorMenu::LayerModeSelectorMenu() { }

		LayerModeSelectorMenu::LayerModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, drawing::Illustrator* illustrator,
			function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler)
			: Menu(startPoint, width, height, visible, nullptr), illustrator(illustrator), touchEventHandler(touchEventHandler) { }

		bool LayerModeSelectorMenu::OnTouch(vec2 point)
		{
			bool touchWasHandled = false;
			if (touchEventHandler != nullptr && visible && boundingRect.Contains(point))
			{
				touchWasHandled = true;
				touchEventHandler(point, this, illustrator);
			}
			return touchWasHandled;
		}
	}
}
