#pragma once
#include "Menu.h"
#include "SymmetryLine.h"

namespace touchpoints { namespace ui
{
	class SymmetryModeSelectorMenu : public Menu
	{
	public:
		SymmetryModeSelectorMenu();
		SymmetryModeSelectorMenu(vec2 startPoint, int height, int width, bool visible,
		    drawing::SymmetryLine* symmetryLine,
		    multimap<int, shared_ptr<drawing::TouchShape>> composingShapes =
		    multimap<int, shared_ptr<drawing::TouchShape>>(),
		    multimap<int, shared_ptr<Menu>> composingMenus = multimap<int, shared_ptr<Menu>>(),
		    function<void(vec2 point, SymmetryModeSelectorMenu* self,
		        drawing::SymmetryLine* symmetryLineLambda)> touchEventHandler = nullptr);
		virtual bool OnTouch(vec2 point) override;
	protected:
		drawing::SymmetryLine* symmetryLine;
		function<void(vec2 point, SymmetryModeSelectorMenu* self, drawing::SymmetryLine* symmetryLineLambda)> touchEventHandler;
	};
}}
