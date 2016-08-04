#pragma once
#include "Menu.h"
#include "UserInterface.h"

namespace touchpoints { namespace ui
{
	class UserInterfaceModeSelectorMenu : public Menu
	{
	public:
		UserInterfaceModeSelectorMenu();
		UserInterfaceModeSelectorMenu(vec2 startPoint, int width, int height, bool visible, shared_ptr<UserInterface> ui,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes = multimap<int, shared_ptr<drawing::TouchShape>>(),
			function<void(vec2 point, UserInterfaceModeSelectorMenu* self, shared_ptr<UserInterface> uiLambda)> touchEventHandler = nullptr);
		bool OnTouch(vec2 point) override;
	private:
		shared_ptr<UserInterface> ui;
		function<void(vec2 point, UserInterfaceModeSelectorMenu* self, shared_ptr<UserInterface> ui)> touchEventHandler;
	};
}}