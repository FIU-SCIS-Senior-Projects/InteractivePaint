#include "SwitchMenu.h"

namespace touchpoints { namespace ui
{
	SwitchMenu::SwitchMenu() { }
	
	SwitchMenu::SwitchMenu(vec2 startPoint, int width, int height, bool visible, 
		bool on, ColorA onColor, ColorA offColor) 
		: Menu(startPoint, width, height, visible, nullptr), on(on), onColor(onColor),
		offColor(offColor)
	{
		auto x1 = startPoint.x;
		auto y1 = startPoint.y;
		auto x2 = x1 + width;
		auto y2 = y1 + height;

		auto initialColor = on ? onColor : offColor;

		rect = drawing::TouchRectangle(x1, y1, x2, y2, initialColor, 0, true);
		greyBorder = drawing::TouchRectangle(x1, y1, x2, y2, Menu::grey, Menu::defaultBorderThickness, false);
	}
	
	void SwitchMenu::OnTouch(vec2 point)
	{
		if(visible && boundingRect.Contains(point))
		{
			on = !on;
		}
	}

	void SwitchMenu::Draw()
	{
		auto currentColor = on ? onColor : offColor;

		rect.setColor(currentColor);
		rect.Draw();
		greyBorder.Draw();
	}
}}