#pragma once
#include "BrushModeSelectorMenu.h"

namespace touchpoints { namespace ui {
	class ShapeModeSelectorMenu : public BrushModeSelectorMenu
	{
	public:
		ShapeModeSelectorMenu();
		ShapeModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
			Shape::Shape shape);
		void Draw() override;
		void OnTouch(vec2 point) override;
	private:
		void setupShapes(Shape::Shape shape);
		Shape::Shape shape;
	};
}}