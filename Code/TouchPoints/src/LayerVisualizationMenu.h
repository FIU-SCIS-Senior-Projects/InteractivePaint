#pragma once
#include "Menu.h"
#include "Brush.h"
#include "BrushModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	class LayerVisualization : public Menu
	{
	public:
		LayerVisualization();
		LayerVisualization(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush,
			function<void(vec2 point, LayerVisualization* self, drawing::Brush* brush)> touchEventHandler);
		LayerVisualization(vec2 startPoint, int width, int height, bool visible, drawing::Brush* brush,
			function<void(vec2 point, LayerVisualization* self, drawing::Brush* brush)> touchEventHandler,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		//LayerVisualization(vec2 startPoint, bool visible, drawing::Brush * brush, Shape::Shape shape);
		/*LayerVisualization(vec2 startPoint, bool visible, drawing::Brush * brush, Shape::Shape shape);
		void Draw();
		void OnTouch(vec2 point) override;
		void setupShapes(Shape::Shape shape);*/
	private:
		drawing::Brush* brush;
		//Shape::Shape shape;
		function<void(vec2 point, LayerVisualization* self, drawing::Brush* brush)> touchEventHandler;
	};
} }
