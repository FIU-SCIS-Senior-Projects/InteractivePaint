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

	void LayerTextureMenu::OnTouch(vec2 point)
	{
		// switches the Active Layer Texture based on which layer was touched
		//if(point.x >= 192 && point.x <= 637)
		if (visible && boundingRect.Contains(point))
		{
			illustrator->MakeLayerActive(index);
		}
		/*if (point.x >= 290 && point.x <= 637)
		{
			if(point.y >=64 && point.y <= 258)
			{
				illustrator->MakeLayerActive(0);
			}
			else if(point.y >= 264 && point.y <= 458)
			{
				illustrator->MakeLayerActive(1);
			}
			else if (point.y >= 464 && point.y <= 658)
			{
				illustrator->MakeLayerActive(2);
			}
		}*/
		//illustrator->MakeLayerActive(index);
	}

	void LayerTextureMenu::setupShapes()
	{
		texture = drawing::TouchTexture(startPoint, defaultWidth,
			LayerModeSelectorMenu::defaultHeight, illustrator->GetLayerTexture(index), illustrator->GetAlpha(index));
	}
}}
