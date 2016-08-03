#include "AlphaChangerMenu.h"


namespace touchpoints { namespace ui
{
	const int AlphaChangerMenu::defaultWidth = 50;
	const int AlphaChangerMenu::numberOfRectangles = 20;
	const int AlphaChangerMenu::alphaCircleRadius = 5;

	AlphaChangerMenu::AlphaChangerMenu() {}

	AlphaChangerMenu::AlphaChangerMenu(vec2 startPoint, bool visible, int index, drawing::Illustrator* illustrator)
		: LayerModeSelectorMenu(startPoint, defaultWidth, LayerVisualizationMenu::defaultHeight, visible,
		illustrator, nullptr), index(index), alphaCircleXPosition(startPoint.x+25)
	{
		setupShapes();
	}

	void AlphaChangerMenu::Draw()
	{
		if(visible)
		{
			Menu::Draw();

			auto currentAlpha = illustrator->GetAlpha(index);
			auto alphaCircleYPosition = startPoint.y + (currentAlpha * height);
			auto newCirclePosition = vec2(alphaCircleXPosition, alphaCircleYPosition);
			currentAlphaCircle.changePosition(newCirclePosition);

			currentAlphaCircle.Draw();
		}
	}

	void AlphaChangerMenu::OnTouch(vec2 point)
	{
		if (visible && boundingRect.Contains(point))
		{
			auto newAlpha = (point.y - startPoint.y) / height;

			illustrator->SetAlpha(index, newAlpha);
		}
	}

	void AlphaChangerMenu::setupShapes()
	{
		auto startX = startPoint.x;
		auto startY = startPoint.y;
		auto endX = startX + width;
		auto endY = startY + 10;
		auto yIncrement = height / numberOfRectangles;
		auto colorDecrement = 1.0 / numberOfRectangles;
		auto color = ColorA(1.0f, 1.0f, 1.0f, 1.0f);

		for (int x = 0; x < numberOfRectangles; x++)
		{
			composingShapes.insert(make_pair(0, shared_ptr<drawing::TouchRectangle>
				(new drawing::TouchRectangle(startX, startY, endX, endY, color, 0, true))));

			startY += yIncrement;
			endY += yIncrement;
			color = ColorA(color.r - colorDecrement, color.g - colorDecrement, color.b - colorDecrement, 1.0f);
		}

		//outline of alpha menu
		composingShapes.insert(make_pair(0, std::make_shared<drawing::TouchRectangle>(startX, startY-3, defaultWidth+240, defaultHeight, Menu::grey,
			Menu::defaultBorderThickness, false)));
		
		auto green = ColorA(0.0f, 1.0f, 0.0f, 1.0f);
		currentAlphaCircle = drawing::TouchCircle(vec2(alphaCircleXPosition, startPoint.y + height), 
			alphaCircleRadius, green, 0, true);
	}
}}
