#include "LayerTextureMenu.h"
#include "LayerVisualizationMenu.h"

namespace touchpoints { namespace ui
{
	const int LayerTextureMenu::defaultWidth = 400;

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

	void LayerTextureMenu::OnTouch(vec2 point)
	{
		illustrator->MakeLayerActive(index);
	}

	void LayerTextureMenu::setupShapes()
	{
		texture = drawing::TouchTexture(startPoint, defaultWidth,
			LayerModeSelectorMenu::defaultHeight, illustrator->GetLayerTexture(index), illustrator->GetAlpha(index));
	}
}}
