#include "LayerTextureMenu.h"
#include "LayerVisualizationMenu.h"

namespace touchpoints { namespace ui
{
	const int LayerTextureMenu::defaultWidth = 400;//345; // 400;

	LayerTextureMenu::LayerTextureMenu() {}

	LayerTextureMenu::LayerTextureMenu(vec2 startPoint, bool visible, int index, drawing::Illustrator* illustrator)
		: LayerModeSelectorMenu(startPoint, defaultWidth, LayerVisualizationMenu::defaultHeight, visible,
			illustrator, nullptr), index(index)
	{
		setupShapes();
	}

	void LayerTextureMenu::Draw()
	{
		if(visible)
		{
			texture.SetTexture(illustrator->GetLayerTexture(index));
			texture.SetAlpha(illustrator->GetAlpha(index));
			texture.Draw();
		}
	}

	bool LayerTextureMenu::OnTouch(vec2 point)
	{
		// switches the Active Layer Texture based on which layer was touched
		bool touchWasHandled = false;
		if (visible && boundingRect.Contains(point))
		{
			touchWasHandled = true;
			illustrator->MakeLayerActive(index);
		}
		return touchWasHandled;
	}

	void LayerTextureMenu::setupShapes()
	{
		texture = drawing::TouchTexture(startPoint, defaultWidth,
			LayerModeSelectorMenu::defaultHeight, illustrator->GetLayerTexture(index), illustrator->GetAlpha(index));
	}
}}
