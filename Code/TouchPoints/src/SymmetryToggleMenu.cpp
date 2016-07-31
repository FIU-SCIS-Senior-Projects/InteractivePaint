#include "SymmetryToggleMenu.h"

namespace touchpoints { namespace ui
{
	SymmetryToggleMenu::SymmetryToggleMenu() {}

	SymmetryToggleMenu::SymmetryToggleMenu(vec2 startPoint, int height, int width, bool visible,
		drawing::SymmetryLine* symmetryLine,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes) 
		: SymmetryModeSelectorMenu(startPoint, width, height, visible, symmetryLine),
		symmetryLineShapes(symmetryLineShapes) { }
	 
	void SymmetryToggleMenu::OnTouch(vec2 point)
	{
		if(boundingRect.Contains(point))
		{
			symmetryLine->toggleSymmetry();
		}
	}

	void SymmetryToggleMenu::Draw()
	{
		if(symmetryLine->getSymmetryOn())
		{
			for (auto zIndexShapePair : symmetryLineShapes)
			{
				zIndexShapePair.second->Draw();
			}
		}
	}
}}