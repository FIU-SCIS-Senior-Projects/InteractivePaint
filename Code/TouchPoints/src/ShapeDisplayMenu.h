#pragma once
#include "BrushModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	class ShapeDisplayMenu : public BrushModeSelectorMenu
	{
	public:
		ShapeDisplayMenu();
		ShapeDisplayMenu(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush);
		void Draw() override;
	private:
		vector<drawing::TouchPoint> lines;
		drawing::TouchCircle filledCircle;
		drawing::TouchCircle outlineCircle;
		drawing::TouchRectangle filledRectangle;
		drawing::TouchRectangle outlineRectangle;
		drawing::TouchVerticalTriangle filledTriangle;
		drawing::TouchVerticalTriangle outlineTriangle;
		drawing::TouchCircle eraser;
		void setupShapes();
	};
}}