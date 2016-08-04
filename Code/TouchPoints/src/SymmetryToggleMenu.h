#pragma once
#include "SymmetryModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	class SymmetryToggleMenu : public SymmetryModeSelectorMenu
	{
	public:
		SymmetryToggleMenu();
		SymmetryToggleMenu(vec2 startPoint, int height, int width, bool visible,
		    drawing::SymmetryLine* symmetryLine,
		    multimap<int, shared_ptr<drawing::TouchShape>> symmetryLineShapes);
		bool OnTouch(vec2 point) override;
		void Draw() override;
	private:
		multimap<int, shared_ptr<drawing::TouchShape>> symmetryLineShapes;
	};
}}
