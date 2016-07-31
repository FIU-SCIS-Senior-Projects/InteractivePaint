#pragma once
#include "Menu.h"
#include "Illustrator.h"

namespace touchpoints { namespace ui
{
	class UndoButton : public Menu
	{
	public:
		UndoButton();
		UndoButton(vec2 startPoint, int height, int width, bool visible,
		           drawing::Illustrator* illustrator);
		void OnTouch(vec2 point) override;
	private:
		drawing::Illustrator* illustrator;
	};
}}
