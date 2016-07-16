#pragma once
#include "Menu.h"
#include "Brush.h"

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
		void OnTouch(vec2 point) override;
	private:
		drawing::Brush* brush;
		function<void(vec2 point, LayerVisualization* self, drawing::Brush* brush)> touchEventHandler;
	};
} }
