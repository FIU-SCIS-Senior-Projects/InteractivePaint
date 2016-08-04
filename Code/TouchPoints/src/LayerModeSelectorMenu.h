#pragma once
#include "Menu.h"
#include "Illustrator.h"

namespace touchpoints { namespace ui
{
	class LayerModeSelectorMenu : public Menu
	{
	public:
		LayerModeSelectorMenu();
		LayerModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, drawing::Illustrator* illustrator,
		    function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler);
		virtual bool OnTouch(vec2 point) override;
	protected:
		drawing::Illustrator* illustrator;
		function<void(vec2 point, LayerModeSelectorMenu* self, drawing::Illustrator* illustrator)> touchEventHandler;
	};
}}
