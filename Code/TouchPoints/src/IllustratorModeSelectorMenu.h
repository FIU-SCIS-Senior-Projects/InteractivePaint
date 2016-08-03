#pragma once
#include "Menu.h"
#include "Illustrator.h"

namespace touchpoints { namespace ui
{
	class IllustratorModeSelectorMenu : public Menu
	{
	public:
		IllustratorModeSelectorMenu();
		IllustratorModeSelectorMenu(vec2 startPoint, int height, int width, bool visible,
			drawing::Illustrator* illustrator,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes =
			multimap<int, shared_ptr<drawing::TouchShape>>(),
			multimap<int, shared_ptr<Menu>> composingMenus = multimap<int, shared_ptr<Menu>>(),
			function<void(vec2 point, IllustratorModeSelectorMenu* self,
				drawing::Illustrator* illustratorLambda)> touchEventHandler = nullptr);
		virtual void OnTouch(vec2 point) override;
	private:
		drawing::Illustrator *illustrator;
		function<void(vec2 point, IllustratorModeSelectorMenu* self, drawing::Illustrator* illustratorLambda)> touchEventHandler;
	};
}}
