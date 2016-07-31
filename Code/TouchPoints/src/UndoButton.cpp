#include "UndoButton.h"

namespace touchpoints { namespace ui
{
	UndoButton::UndoButton() { }

	UndoButton::UndoButton(vec2 startPoint, int height, int width, bool visible,
		drawing::Illustrator* illustrator) : Menu(startPoint, height, width, visible, nullptr),
		illustrator(illustrator) { }

	void UndoButton::OnTouch(vec2 point)
	{
		if(boundingRect.Contains(point))
		{
			illustrator->Undo();
		}
	}
}}