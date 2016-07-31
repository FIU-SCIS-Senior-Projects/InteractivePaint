#include "Menu.h"
#include "TouchRectangle.h"
#include "TouchImage.h"

namespace touchpoints { namespace ui
{
	const ColorA Menu::grey = ColorA(0.75f, 0.75f, 0.75f, 1.0f);
	const ColorA Menu::purple = ColorA(0.3f, 0.2f, 0.5f, 1.0f);
	const ColorA Menu::white = ColorA(1.0f, 1.0f, 1.0f, 1.0f);
	const ColorA Menu::black = ColorA(0.0f, 0.0f, 0.0f, 1.0f);
	const ColorA Menu::green = ColorA(0.0f, 1.0f, 0.0f, 1.0f);
	const function<void(vec2 point, Menu *self)> Menu::defaultDropdownCallback =
		[](vec2 point, Menu *self) { self->ToggleContainingMenusVisibility(); };
	const function<void(vec2 point, Menu *self)> Menu::recursiveDropdownCallback =
		[](vec2 point, Menu *self) { self->ToggleContainingMenusVisibilityRecursively(); };

	Menu::Menu() { }

	Menu::Menu(vec2 startPoint, bool visible, function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, defaultWidth, defaultHeight, visible,
			multimap<int, shared_ptr<drawing::TouchShape>>(), multimap<int, shared_ptr<Menu>>(),
			touchEventHandler) { }

	Menu::Menu(vec2 startPoint, bool visible, ColorA backgroundColor, string imagePath,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, defaultWidth, defaultHeight, visible,
			multimap<int, shared_ptr<drawing::TouchShape>>(), multimap<int, shared_ptr<Menu>>(),
			touchEventHandler)
	{
		populateDefaultShapes(backgroundColor, imagePath);
	}

	Menu::Menu(vec2 startPoint, int width, int height, bool visible,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, width, height, visible, multimap<int, shared_ptr<drawing::TouchShape>>(), 
			multimap<int, shared_ptr<Menu>>(), touchEventHandler) { }

	Menu::Menu(vec2 startPoint, bool visible,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes)
		: Menu(startPoint, defaultWidth, defaultHeight, visible,
			composingShapes, multimap<int, shared_ptr<Menu>>(), nullptr) { }

	Menu::Menu(vec2 startPoint, int width, int height, bool visible,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, width, height, visible, composingShapes,
			multimap<int, shared_ptr<Menu>>(), touchEventHandler) { }

	Menu::Menu(vec2 startPoint, bool visible,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, defaultWidth, defaultHeight, visible,
			multimap<int, shared_ptr<drawing::TouchShape>>(),
			composingMenus, touchEventHandler) { }

	Menu::Menu(vec2 startPoint, bool visible, ColorA backgroundColor, string imagePath,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, defaultWidth, defaultHeight, visible,
			multimap<int, shared_ptr<drawing::TouchShape>>(),
			composingMenus, touchEventHandler)
	{
		populateDefaultShapes(backgroundColor, imagePath);
	}

	Menu::Menu(vec2 startPoint, bool visible, ColorA backgroundColor, string imagePath,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, defaultWidth, defaultHeight, visible,
			composingShapes, composingMenus, touchEventHandler)
	{
		populateDefaultShapes(backgroundColor, imagePath);
	}

	Menu::Menu(vec2 startPoint, int width, int height, bool visible,
		multimap<int, shared_ptr<Menu>> composingMenus,
		function<void(vec2 point, Menu* self)> touchEventHandler)
		: Menu(startPoint, width, height, visible, multimap<int, shared_ptr<drawing::TouchShape>>(),
			composingMenus, touchEventHandler) { }

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

	void Menu::ToggleContainingMenusVisibilityRecursively()
	{
		for (auto zIndexMenuPair : composingMenus)
		{
			auto composingMenu = zIndexMenuPair.second;
			composingMenu->ToggleVisiblibility();

			if(composingMenu->composingMenus.size() > 0)
			{
				composingMenu->ToggleContainingMenusVisibilityRecursively();
			}
		}
	}

	void Menu::initilizeBoundingRect()
	{
		auto endPoint = vec2(startPoint.x + width, startPoint.y + height);
		boundingRect = math::Rect(startPoint, endPoint);
	}

	void Menu::populateDefaultShapes(ColorA backgroundColor = black, string imagePath = "")
	{
		auto startX = startPoint.x;
		auto startY = startPoint.y;
		auto endX = startX + defaultWidth;
		auto endY = startY + defaultHeight;
		auto imageStart = vec2(startX + defaultImageOffsetX, startY + defaultImageOffsetY);

		AddShape(0, shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(startX, startY, endX, endY, backgroundColor, 0, true)));
		AddShape(1, shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(startX, startY, endX, endY, grey, defaultBorderThickness, false)));
		if (imagePath != "")
		{
			AddShape(2, shared_ptr<drawing::TouchImage>(new drawing::TouchImage(imageStart, defaultImageWidth, defaultImageHeight, imagePath)));
		}
	}
}}
