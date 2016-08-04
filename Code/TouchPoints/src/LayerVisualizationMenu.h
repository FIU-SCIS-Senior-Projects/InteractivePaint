#pragma once
#include "LayerModeSelectorMenu.h"
#include "Illustrator.h"

namespace touchpoints { namespace ui
{
	class LayerVisualizationMenu : public LayerModeSelectorMenu
	{
	public:
		LayerVisualizationMenu();
		LayerVisualizationMenu(vec2 startPoint, int width, int height, int layerIndex, bool visible, drawing::Illustrator* illustrator,
			function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler);
		bool OnTouch(vec2 point) override;
		static const int defaultWidth;
		static const int defaultHeight;
		float startY1;
		float startY2;
		float startY3;
		const int lWidth;
		const int lHeight;
		const int index;
		void Draw() override;
	private:
		void setupShapes();
		void setupMenus();
	};
} }
