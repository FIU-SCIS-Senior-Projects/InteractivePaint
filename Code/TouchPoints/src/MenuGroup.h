#pragma once
#include "Menu.h"

namespace touchpoints { namespace ui
{
	class MenuGroup : public Menu
	{
	public:
		MenuGroup();
		MenuGroup(multimap<int, shared_ptr<IMenu>> composingMenus);
		MenuGroup(multimap<int, shared_ptr<drawing::TouchShape>> composingShapes, 
			multimap<int, shared_ptr<IMenu>> composingMenus);
		void Draw() override;
		void OnTouch(vec2 point) override;
		void AddMenu(int zIndex, shared_ptr<IMenu> menu);
		void AddShape(int zIndex, shared_ptr<drawing::TouchShape> shape);
	protected:
		multimap<int, shared_ptr<IMenu>> composingMenus;
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes;
	};
}}