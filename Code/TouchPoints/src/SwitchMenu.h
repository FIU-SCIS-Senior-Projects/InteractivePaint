#pragma once
#include "Menu.h"
#include "TouchRectangle.h"

namespace touchpoints { namespace ui
{
	class SwitchMenu : public Menu
	{
	public:
		SwitchMenu();
		SwitchMenu(vec2 startPoint, int width = 60, int height = 60, bool visible = false,
			bool on = false, ColorA onColor = Menu::green, ColorA offColor = Menu::black);
		virtual void OnTouch(vec2 point) override;
		void Draw() override;
	protected:
		bool on;
		drawing::TouchRectangle rect;
		drawing::TouchRectangle greyBorder;
		ColorA onColor;
		ColorA offColor;
	};
}}
