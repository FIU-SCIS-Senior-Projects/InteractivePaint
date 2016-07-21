#include "LayerVisualizationMenu.h"
#include "Menu.h"

namespace touchpoints { namespace ui
{
	const int LayerVisualizationMenu::defaultWidth = 450;
	const int LayerVisualizationMenu::defaultHeight = 200;

	LayerVisualizationMenu::LayerVisualizationMenu() { }

	LayerVisualizationMenu::LayerVisualizationMenu(vec2 startPoint, bool visible, drawing::Illustrator* illustrator, 
		function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler)
		: LayerModeSelectorMenu(startPoint, defaultWidth, defaultHeight, visible, illustrator, touchEventHandler)
	{
		setupShapes();
		setupMenus();
	}

	void LayerVisualizationMenu::OnTouch(vec2 point)
	{
		//make sure that the x menu is the first to respond to the event
		//we might need to have the on touch return a boolean depending on whether 
		//it responded to the touch event or not
		//that way we can terminate the on touch for the whole menu if the x is touched

		if (touchEventHandler != nullptr && visible && boundingRect.Contains(point))
		{
			touchEventHandler(point, this, illustrator);
		}
	}

	void LayerVisualizationMenu::setupShapes()
	{
		//grey rectangle around(?)
	}

	void LayerVisualizationMenu::setupMenus()
	{
		//contains x menu
		//createAlphaMenu
		//contains layerTexture menu
	}
} }
