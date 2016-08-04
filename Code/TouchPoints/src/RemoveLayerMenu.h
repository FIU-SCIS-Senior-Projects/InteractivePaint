#pragma once
#include "LayerModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	class RemoveLayerMenu : public LayerModeSelectorMenu
	{
	public:
		RemoveLayerMenu();
		RemoveLayerMenu(vec2 startPoint, bool visible, int index, drawing::Illustrator* illustrator);
		bool OnTouch(vec2 point) override;
	private:
		static const int defaultWidth;
		static const int defaultHeight;
		int index;
		void setupShapes();
	};
}}
