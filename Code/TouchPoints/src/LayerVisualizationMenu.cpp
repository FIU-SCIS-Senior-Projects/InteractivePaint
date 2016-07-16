#include "LayerVisualizationMenu.h"
#include "Menu.h"

namespace touchpoints { namespace ui
{
	LayerVisualization::LayerVisualization() { }

	LayerVisualization::LayerVisualization(vec2 startPoint, int width, int height, bool visible,
		drawing::Brush* brush, function<void(vec2 point, LayerVisualization* self, drawing::Brush* brush)> touchEventHandler)
		: Menu(startPoint, width, height, visible, nullptr), brush(brush), touchEventHandler(touchEventHandler) { }

	LayerVisualization::LayerVisualization(vec2 startPoint, int width, int height, bool visible,
		drawing::Brush* brush, function<void(vec2 point, LayerVisualization* self, drawing::Brush* brush)> touchEventHandler,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: Menu(startPoint, width, height, visible, composingShapes, nullptr), brush(brush),
		touchEventHandler(touchEventHandler) { }

	void LayerVisualization::OnTouch(vec2 point)
	{
		if (touchEventHandler != nullptr && visible && boundingRect.Contains(point))
		{
			touchEventHandler(point, this, brush);
		}
	}

} }
