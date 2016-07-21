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

	void RemoveLayerMenu::OnTouch(vec2 point)
	{
		if (visible && boundingRect.Contains(point))
		{
			illustrator->DeleteLayer(index);
		}
	}

	void RemoveLayerMenu::setupShapes()
	{
		composingShapes.insert(make_pair(0, shared_ptr<drawing::TouchImage>(new drawing::TouchImage(startPoint, defaultWidth, defaultHeight, "close.png"))));
	}
}}
