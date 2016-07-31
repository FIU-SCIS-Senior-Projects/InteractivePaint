#include "LayerVisualizationMenu.h"
#include "Menu.h"
#include "LayerTextureMenu.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/scoped.h>

namespace touchpoints { namespace ui
{
	const int LayerVisualizationMenu::defaultWidth = 400;//450;
	const int LayerVisualizationMenu::defaultHeight = 200;//194;//200;

	LayerVisualizationMenu::LayerVisualizationMenu(): startY1(0), startY2(0), startY3(0), lWidth(0), lHeight(0), index(0)
	{ }

	LayerVisualizationMenu::LayerVisualizationMenu(vec2 startPoint, int width, int height, int layerIndex, bool visible, drawing::Illustrator* illustrator, 
		function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler)
		: LayerModeSelectorMenu(startPoint, width, height, visible, illustrator, touchEventHandler), lWidth(width), lHeight(height), index(layerIndex)
	{
		setupShapes();
		setupMenus();
	}

	void LayerVisualizationMenu::Draw()
	{
		if (!visible)
		{
			return;
		}

		for(int i = 0; i < composingShapes.size(); i++)
		{
			auto temp = composingShapes.find(i)->second;
			temp->Draw();
		}

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
		auto startX = startPoint.x-50;
		auto startY = startPoint.y;
		ColorA green = ColorA(0.0, 1.00f, 0.0, 1.00f);
		composingShapes.insert(make_pair(0, shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(startX, startY, lWidth, lHeight, green, 0, true))));
		composingShapes.insert(make_pair(1, shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(startX, startY, lWidth, lHeight, Menu::grey, Menu::defaultBorderThickness, false))));
	}

	void LayerVisualizationMenu::setupMenus()
	{
		auto startX = startPoint.x-50;
		auto startY = startPoint.y;

		//contains x menu
		auto closeImageStartPoint = vec2(lWidth + Menu::defaultImageOffsetX - 56, lHeight + Menu::defaultImageOffsetY - 3);
		composingShapes.insert(make_pair(2, std::make_shared<drawing::TouchImage>(closeImageStartPoint, Menu::defaultImageWidth,
		                                                                            Menu::defaultImageHeight, "close.png")));

		// + button to add more layers
		if (index == illustrator->GetNumberOfLayersInCanvas() - 1)
		{
			auto imageStartPoint = vec2(lWidth + Menu::defaultImageOffsetX - 58, lHeight + Menu::defaultImageOffsetY + 143);
			composingShapes.insert(make_pair(3, std::make_shared<drawing::TouchImage>(imageStartPoint, Menu::defaultImageWidth,
			                                                                            Menu::defaultImageHeight, "smallPlus.png")));
		}

	}
} }
