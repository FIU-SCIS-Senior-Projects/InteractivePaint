#include "Menu.h"

namespace touchpoints { namespace ui
{
	Menu::Menu() {}
	
	Menu::Menu(int width, int height) : width(width), height(height) { }
	
	Menu::Menu(int width, int height, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: width(width), height(height), composingShapes(composingShapes) { }
	
	void Menu::Draw()
	{
		for (auto zIndexShapePair : composingShapes)
		{
			zIndexShapePair.second->draw();
		}
	}
}}
