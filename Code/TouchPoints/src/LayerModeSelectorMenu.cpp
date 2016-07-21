#include "LayerModeSelectorMenu.h"
#include "Menu.h"

namespace touchpoints {
	namespace ui
	{
		LayerModeSelectorMenu::LayerModeSelectorMenu() { }

		LayerModeSelectorMenu::LayerModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, drawing::Illustrator* illustrator,
			function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler)
			: Menu(startPoint, defaultWidth, defaultHeight, visible, nullptr), illustrator(illustrator), touchEventHandler(touchEventHandler) { }

		void LayerModeSelectorMenu::OnTouch(vec2 point)
		{
			if (touchEventHandler != nullptr && visible && boundingRect.Contains(point))
			{
				touchEventHandler(point, this, illustrator);
			}
		}
	}
}
