#pragma once
#include "LayerModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	class AlphaChangerMenu : public LayerModeSelectorMenu
	{
	public:
		AlphaChangerMenu();
		AlphaChangerMenu(vec2 startPoint, bool visible, int index, drawing::Illustrator* illustrator);
		void Draw() override;
		void OnTouch(vec2 point) override;
	private:
		static const int defaultWidth;
		static const int numberOfRectangles;
		static const int alphaCircleRadius;
		int index;
		float alphaCircleXPosition;
		drawing::TouchCircle currentAlphaCircle;
		void setupShapes();
	};
}}
