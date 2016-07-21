#pragma once
#include "LayerModeSelectorMenu.h"
#include "Illustrator.h"

namespace touchpoints { namespace ui
{
	class LayerVisualizationMenu : public LayerModeSelectorMenu
	{
	public:
		LayerVisualizationMenu();
		LayerVisualizationMenu(vec2 startPoint, bool visible, drawing::Illustrator* illustrator,
			function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler);
		void OnTouch(vec2 point) override;
		static const int defaultWidth;
		static const int defaultHeight;
	private:
		void setupShapes();
		void setupMenus();
	};
} }
