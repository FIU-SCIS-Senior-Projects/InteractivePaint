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
		startY1 = startPoint.y - 7;
		startY2 = startPoint.y - 7;
		startY3 = startPoint.y - 7;
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

		//if(initIndicator < 3)
		//{
		//	gl::color(0.0, 1.0f, 0.0, 1.0f);
		//	gl::drawSolidCircle(vec2(265, (startY1 - 200) + illustrator->GetAlpha(index) * (startY1 - (startY1 - 200))), 5.0f);
		//	initIndicator++;
		//}
		//else
		//{
			gl::color(0.0, 1.0f, 0.0, 1.0f);
			gl::drawSolidCircle(vec2(265, (startY1 - 200) + illustrator->GetAlpha(index) * (startY1 - (startY1 - 200))), 5.0f);

			gl::color(0.0, 1.0f, 0.0, 1.0f);
			gl::drawSolidCircle(vec2(265, (startY2 - 200) + illustrator->GetAlpha(index) * (startY2 - (startY2 - 200))), 5.0f);

			gl::color(0.0, 1.0f, 0.0, 1.0f);
			gl::drawSolidCircle(vec2(265, (startY3 - 200) + illustrator->GetAlpha(index) * (startY3 - (startY3 - 200))), 5.0f);
		//}

	}

	void LayerVisualizationMenu::OnTouch(vec2 point)
	{
		//make sure that the x menu is the first to respond to the event
		//we might need to have the on touch return a boolean depending on whether 
		//it responded to the touch event or not
		//that way we can terminate the on touch for the whole menu if the x is touched

		// Draws indicator for alpha level
		//based on alpha menu touch, redraw the alpha indicator in the corrent location
		// therefore, change the y value based on touch
		if (point.x >= 240 && point.x <= 290)
		{
			if (point.y >= 60 && point.y <= 260)
			{
				startY1 = point.y;
				//illustrator->SetAlpha(0, 0.75f);
			}
			else if (point.y >= 260 && point.y <= 460)
			{
				startY2 = point.y;
			}
			else if (point.y >= 460 && point.y <= 660)
			{
				startY3 = point.y;
			}
		}

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

		// Draws the different shades of grey for the layers alpha bar
		// z is 3 through 10
		ColorA alphaBarGreys;
		int z = 3;
		for (int x = 0; x < 8; x++)
		{
			alphaBarGreys = ColorA(0.75f, 0.75f, 0.75f, 0.75f + .035 * x);
			composingShapes.insert(make_pair(z, std::make_shared<drawing::TouchRectangle>(startX, startY - 195 + x * 24, lWidth - 350, startY - 197 + (x + 1) * 25,
			                                                                                alphaBarGreys, 0, true)));
			z++;
		}

		//createAlphaMenu
		// Draws gray bar to separate the layer alpha bar from the layer display
		composingShapes.insert(make_pair(z, std::make_shared<drawing::TouchRectangle>(startX, startY, lWidth-350, lHeight, Menu::grey,
			Menu::defaultBorderThickness, false)));
		z++;
		// Draws indicator for alpha level
		/*auto radius = 5.00f;
		auto center = vec2(265, startY - 7);
		ColorA green = ColorA(0.0, 1.00f, 0.0, 1.00f);
		composingShapes.insert(make_pair(z, std::make_shared<drawing::TouchCircle>(center, radius, green, 0, true)));
		z++;*/

		// + button to add more layers
		if (index == illustrator->GetNumberOfLayersInCanvas() - 1)
		{
			auto imageStartPoint = vec2(lWidth + Menu::defaultImageOffsetX - 58, lHeight + Menu::defaultImageOffsetY + 143);
			composingShapes.insert(make_pair(z, std::make_shared<drawing::TouchImage>(imageStartPoint, Menu::defaultImageWidth,
			                                                                            Menu::defaultImageHeight, "smallPlus.png")));
		}
		z++;

		//contains layerTexture menu
		//composingShapes.insert(make_pair(z, illustrator->GetLayerTexture(index)));

		//composingShapes.insert(make_pair(z, shared_ptr<LayerTextureMenu>(new LayerTextureMenu(startPoint, true, index, illustrator))));
		//auto layerTexture = std::make_shared<LayerTextureMenu>(startPoint, width, height, true, index, illustrator);
		//composingShapes.insert(make_pair(z, layerTexture));

		//auto layerTexture = std::make_shared<LayerTextureMenu>(startPoint, true, index, illustrator);
		//composingShapes.insert(make_pair(14, shared_ptr<LayerTextureMenu>(new LayerTextureMenu(startPoint, true, index, illustrator)));
		//layerPickerShapes.insert(make_pair(4, shared_ptr<LayerTextureMenu> (new LayerTextureMenu(vec2(x2, y2), false, layerIndex, illustrator)));
		//LayerTextureMenu texture = LayerTextureMenu(startPoint, true, index, illustrator);
		//composingShapes.insert(make_pair(14, std::make_shared<LayerTextureMenu>(startPoint, true, index, illustrator)));

	}
} }
