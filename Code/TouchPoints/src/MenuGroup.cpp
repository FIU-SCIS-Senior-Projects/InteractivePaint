#include "MenuGroup.h"

namespace touchpoints { namespace ui {
	MenuGroup::MenuGroup() {}

	MenuGroup::MenuGroup(multimap<int, shared_ptr<IMenu>> composingMenus)
		: composingShapes(multimap<int, shared_ptr<drawing::TouchShape>>()), composingMenus(composingMenus) {}

	MenuGroup::MenuGroup(multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		multimap<int, shared_ptr<IMenu>> composingMenus) : 
		composingShapes(composingShapes), composingMenus(composingMenus) {}

	void MenuGroup::Draw()
	{
		for (auto zIndexShapePair : composingShapes)
		{
			zIndexShapePair.second->Draw();
		}

		for(auto zIndexMenuPair : composingMenus)
		{
			zIndexMenuPair.second->Draw();
		}
	}
	void MenuGroup::OnTouch(vec2 point)
	{
		for (auto zIndexMenuPair : composingMenus)
		{
			zIndexMenuPair.second->OnTouch(point);
		}
	}

	void MenuGroup::AddMenu(int zIndex, shared_ptr<IMenu> menu)
	{
		composingMenus.insert(pair<int, shared_ptr<IMenu>>(zIndex, menu));
	}

	void MenuGroup::AddShape(int zIndex, shared_ptr<drawing::TouchShape> shape)
	{
		composingShapes.insert(pair<int, shared_ptr<drawing::TouchShape>>(zIndex, shape));
	}
}}