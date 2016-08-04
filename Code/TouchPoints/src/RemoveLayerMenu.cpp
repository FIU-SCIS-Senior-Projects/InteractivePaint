#include "RemoveLayerMenu.h"

namespace touchpoints { namespace ui
{
	const int RemoveLayerMenu::defaultWidth = 10;
	const int RemoveLayerMenu::defaultHeight = 10;

	RemoveLayerMenu::RemoveLayerMenu(vec2 startPoint, bool visible, int index, drawing::Illustrator* illustrator)
		: LayerModeSelectorMenu(startPoint, defaultWidth, defaultHeight, visible, illustrator, nullptr), index(index)
	{
		setupShapes();
	}

	bool RemoveLayerMenu::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		if (visible && boundingRect.Contains(point))
		{
			touchWasHandled = true;
			illustrator->DeleteLayer(index);
		}
		return touchWasHandled;
	}

	void RemoveLayerMenu::setupShapes()
	{
		composingShapes.insert(make_pair(0, shared_ptr<drawing::TouchImage>(new drawing::TouchImage(startPoint, defaultWidth, defaultHeight, "close.png"))));
	}
}}
