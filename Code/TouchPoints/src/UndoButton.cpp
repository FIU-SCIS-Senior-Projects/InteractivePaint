#include "UndoButton.h"

namespace touchpoints { namespace ui
{
	UndoButton::UndoButton() { }

	UndoButton::UndoButton(vec2 startPoint, int height, int width, bool visible,
		drawing::Illustrator* illustrator) : Menu(startPoint, height, width, visible, nullptr),
		illustrator(illustrator) { }

	bool UndoButton::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		if(boundingRect.Contains(point))
		{
			touchWasHandled = true;
			illustrator->Undo();
		}
		return touchWasHandled;
	}
}}