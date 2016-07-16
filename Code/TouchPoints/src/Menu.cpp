#include "Menu.h"

namespace touchpoints { namespace ui
{
	const ColorA Menu::grey = ColorA(0.75f, 0.75f, 0.75f, 1.0f);
	const ColorA Menu::purple = ColorA(0.3f, 0.2f, 0.5f, 1.0f);
	const ColorA Menu::white = ColorA(1.0f, 1.0f, 1.0f, 1.0f);
	const function<void(vec2 point, Menu *self)> Menu::defaultDropdownCallback =
		[](vec2 point, Menu *self) { self->ToggleContainingMenusVisibility(); };

	Menu::Menu() {}

	Menu::Menu(vec2 startPoint, int width, int height, bool visible,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, width, height, visible, multimap<int, shared_ptr<drawing::TouchShape>>(), 
			multimap<int, shared_ptr<Menu>>(), touchEventHandler) {}

	Menu::Menu(vec2 startPoint, int width, int height, bool visible,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, width, height, visible, composingShapes,
			multimap<int, shared_ptr<Menu>>(), touchEventHandler) {}

	Menu::Menu(vec2 startPoint, int width, int height, bool visible,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, width, height, visible, multimap<int, shared_ptr<drawing::TouchShape>>(),
			composingMenus, touchEventHandler) {}

	Menu::Menu(vec2 startPoint, int width, int height, bool visible,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: visible(visible), startPoint(startPoint), width(width), height(height),
		composingShapes(composingShapes), composingMenus(composingMenus), 
		touchEventHandler(touchEventHandler)
	{
		initilizeBoundingRect();
	}

	void Menu::Draw()
	{
		if(!visible)
		{
			return;
		}

		for (auto zIndexShapePair : composingShapes)
		{
			zIndexShapePair.second->Draw();
		}

		for (auto zIndexMenuPair : composingMenus)
		{
			zIndexMenuPair.second->Draw();
		}
	}

	void Menu::OnTouch(vec2 point)
	{
		if (touchEventHandler != nullptr && visible && boundingRect.Contains(point))
		{
			touchEventHandler(point, this);
		}

		for (auto zIndexMenuPair : composingMenus)
		{
			zIndexMenuPair.second->OnTouch(point);
		}
	}

	void Menu::AddShape(int zIndex, shared_ptr<drawing::TouchShape> shape)
	{
		composingShapes.insert(pair<int, shared_ptr<drawing::TouchShape>>(zIndex, shape));
	}

	void Menu::AddMenu(int zIndex, shared_ptr<Menu> menu)
	{
		composingMenus.insert(pair<int, shared_ptr<Menu>>(zIndex, menu));
	}

	void Menu::ToggleContainingMenusVisibility()
	{
		for (auto zIndexMenuPair : composingMenus)
		{
			zIndexMenuPair.second->ToggleVisiblibility();
		}
	}

	void Menu::initilizeBoundingRect()
	{
		auto endPoint = vec2(startPoint.x + width, startPoint.y + height);
		boundingRect = math::Rect(startPoint, endPoint);
	}
}}
