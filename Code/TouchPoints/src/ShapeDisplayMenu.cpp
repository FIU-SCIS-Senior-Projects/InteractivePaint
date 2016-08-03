#include "ShapeDisplayMenu.h"
#include "Line.h"

namespace touchpoints { namespace ui
{
	ShapeDisplayMenu::ShapeDisplayMenu() { }

	ShapeDisplayMenu::ShapeDisplayMenu(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush)
		: BrushModeSelectorMenu(startPoint, width, height, visible, brush, nullptr)
	{
		setupShapes();
	}

	void ShapeDisplayMenu::Draw()
	{
		auto currentColor = brush->getColor();
		auto currentLineSize = brush->getLineSize();
		auto shapesAreFilled = brush->getFilledShapes();
		switch(brush->getShape())
		{
			case Shape::Line :
			{
				for(auto line : lines)
				{
					line.setSize(currentLineSize);
					line.setColor(currentColor);
					line.Draw();
				}
				break;
			}
			case Shape::Circle :
			{
				if (shapesAreFilled)
				{
					filledCircle.setColor(currentColor);
					filledCircle.Draw();
				}
				else
				{
					outlineCircle.setColor(currentColor);
					outlineCircle.setSize(currentLineSize);
					outlineCircle.Draw();
				}
				break;
			}
			case Shape::Rectangle :
			{
				if (shapesAreFilled)
				{
					filledRectangle.setColor(currentColor);
					filledRectangle.Draw();
				}
				else
				{
					outlineRectangle.setColor(currentColor);
					outlineRectangle.setSize(currentLineSize);
					outlineRectangle.Draw();
				}
				break;
			}
			case Shape::Triangle :
			{
				if (shapesAreFilled)
				{
					filledTriangle.setColor(currentColor);
					filledTriangle.Draw();
				}
				else
				{
					outlineTriangle.setColor(currentColor);
					outlineTriangle.setSize(currentLineSize);
					outlineTriangle.Draw();
				}
				break;
			}
			case Shape::Eraser :
			{
				eraser.Draw();
				break;
			}
		}
	}


	void ShapeDisplayMenu::setupShapes()
	{
		auto currentColor = brush->getColor();
		auto currentLineSize = brush->getLineSize();
	
		auto x1 = startPoint.x;
		auto y1 = startPoint.y;
		auto x2 = startPoint.x + width;
		auto y2 = startPoint.y + height;
		auto endPoint = vec2(x2, y2);
		auto topRightCorner = vec2(x2, y1);
		auto bottomLeftCorner = vec2(x1, y2);
		auto midYLeft = math::FindMidPoint(startPoint, bottomLeftCorner);
		auto midYRight = math::FindMidPoint(topRightCorner, endPoint);
		auto midXTop = math::FindMidPoint(startPoint, topRightCorner);
		auto midXBottom = math::FindMidPoint(bottomLeftCorner, endPoint);
		lines = vector<drawing::TouchPoint>
		{
			drawing::TouchPoint(midYLeft, midXTop, currentColor, currentLineSize),
			drawing::TouchPoint(bottomLeftCorner, topRightCorner, currentColor, currentLineSize),
			drawing::TouchPoint(midXBottom, midYRight, currentColor, currentLineSize)
		};

		auto circleCenter = math::FindMidPoint(startPoint, endPoint);
		filledCircle = drawing::TouchCircle(circleCenter,width/2, currentColor, 0, true);
		outlineCircle = drawing::TouchCircle(circleCenter,width/2, currentColor, currentLineSize, false);
		filledRectangle = drawing::TouchRectangle(x1, y1, x2, y2, currentColor, 0, true);
		outlineRectangle = drawing::TouchRectangle(x1, y1, x2, y2, currentColor, currentLineSize, false);
		auto baseLeft = vec2(x1, y2);
		auto baseRight = vec2(x2, y2);
		filledTriangle = drawing::TouchVerticalTriangle(baseLeft, baseRight, midXTop, midXBottom, currentColor,
			0, true);
		outlineTriangle = drawing::TouchVerticalTriangle(baseLeft, baseRight, midXTop, midXBottom, currentColor,
			currentLineSize, false);
		eraser = drawing::TouchCircle(circleCenter, width / 10, Menu::white, currentLineSize, false);
	}
}}
