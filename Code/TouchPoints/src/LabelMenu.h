#pragma once
#include "Menu.h"

namespace touchpoints { namespace ui
{
	class LabelMenu : public Menu
	{
	public:
		LabelMenu(vec2 startPoint, int width, int height, bool visible, std::string text,
			int fontSize = 200, std::string font = "Arial", ColorA textColor = Menu::white,
			ColorA backgroundColor = Menu::black);
	};
}}
