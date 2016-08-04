#include "LabelMenu.h"
#include "TouchRectangle.h"
#include "TouchTextLayout.h"

namespace touchpoints { namespace ui
{
	LabelMenu::LabelMenu(vec2 startPoint, int width, int height, bool visible, std::string text,
		int fontSize, std::string font, ColorA textColor, ColorA backgroundColor)
		: Menu(startPoint, width, height, visible,
			multimap<int, shared_ptr<drawing::TouchShape>>(),
			multimap<int, shared_ptr<Menu>>(), nullptr)
	{
		auto x1 = startPoint.x;
		auto y1 = startPoint.y;
		auto x2 = x1 + width;
		auto y2 = y1 + height;
		auto backgroundRectangle = shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(
			x1, y1, x2, y2, backgroundColor, 0, true));
		auto greyBorder = shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(
			x1, y1, x2, y2, Menu::grey, Menu::defaultBorderThickness, false));
		auto textLayout = shared_ptr<drawing::TouchTextLayout>(new drawing::TouchTextLayout(
		startPoint, width, height, text, font, textColor, fontSize));

		composingShapes.insert(make_pair(0, backgroundRectangle));
		composingShapes.insert(make_pair(1, greyBorder));
		composingShapes.insert(make_pair(1, textLayout));
	}
}}
