#pragma once
#include "Menu.h"

namespace touchpoints { namespace ui
{
	class TouchMenu : public Menu
	{
	public:
		TouchMenu();
		TouchMenu(vec2 startPoint, int width, int height, bool visible, function<void(vec2 point, TouchMenu *self)> touchEventHandler);
		TouchMenu(vec2 startPoint, int width, int height, bool visible, function<void(vec2 point,TouchMenu *self)> touchEventHandler,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		void OnTouch(vec2 point) override;
	private:
		function<void(vec2 point, TouchMenu *self)> touchEventHandler;
	};
}}