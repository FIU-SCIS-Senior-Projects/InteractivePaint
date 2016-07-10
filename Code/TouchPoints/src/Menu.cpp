#include "Menu.h"

namespace touchpoints { namespace ui
{
	Menu::Menu() {}
	
	Menu::Menu(vec2 startPoint, int width, int height, bool visible)
		: startPoint(startPoint), width(width), height(height), visible(visible)
	{
		initilizeBoundingRect();
	}
	
	Menu::Menu(vec2 startPoint, int width, int height, bool visible, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: startPoint(startPoint), width(width), height(height), visible(visible) 
	{
		initilizeBoundingRect();
	}
	
	void Menu::Draw()
	{
		if(!visible)
		{
			return;
		}

		for (auto zIndexShapePair : composingShapes)
		{
			zIndexShapePair.second->Draw();
		}
	}

	void Menu::initilizeBoundingRect()
	{
		auto endPoint = vec2(startPoint.x + width, startPoint.y + height);
		boundingRect = math::Rect(startPoint, endPoint);
	}
}}
