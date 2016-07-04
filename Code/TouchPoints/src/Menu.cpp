#include "Menu.h"

namespace touchpoints { namespace ui
{
	Menu::Menu() {}
	
	Menu::Menu(int width, int height, bool visible) : width(width), height(height), visible(visible) { }
	
	Menu::Menu(int width, int height, bool visible, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: width(width), height(height), visible(visible), composingShapes(composingShapes) { }
	
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
}}
