#pragma once
#include "LayerModeSelectorMenu.h"

namespace touchpoints { namespace ui
{
	class LayerTextureMenu : public LayerModeSelectorMenu
	{
	public:
		LayerTextureMenu();
		LayerTextureMenu(vec2 startPoint, bool visible, int index, drawing::Illustrator* illustrator);
		void Draw() override;
		void OnTouch(vec2 point) override;
	private:
		static const int defaultWidth;
		int index;
		drawing::TouchTexture texture;
		void setupShapes();
	};
}}
