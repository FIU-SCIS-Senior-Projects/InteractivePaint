#include "ModeSelectorMenu.h"
#include "TouchRectangle.h"
#include "TouchImage.h"

namespace touchpoints { namespace ui {
	const ColorA ModeSelectorMenu::grey = ColorA(0.75f, 0.75f, 0.75f, 1.0f);
	const ColorA ModeSelectorMenu::purple = ColorA(0.3f, 0.2f, 0.5f, 1.0f);

	ModeSelectorMenu::ModeSelectorMenu() {}

	ModeSelectorMenu::ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
		function<void(vec2 point, Menu *self)> touchEventHandler) : ModeSelectorMenu(startPoint, filePath,
		visible, multimap<int, shared_ptr<drawing::TouchShape>>(), multimap<int, shared_ptr<Menu>>(), touchEventHandler) {}

	ModeSelectorMenu::ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		function<void(vec2 point, Menu *self)> touchEventHandler) 
		: ModeSelectorMenu(startPoint, filePath, visible, composingShapes,
			multimap<int, shared_ptr<Menu>>(), touchEventHandler) {}

	ModeSelectorMenu::ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
		multimap<int, shared_ptr<Menu>> composingMenus, 
		function<void(vec2 point, Menu *self)> touchEventHandler) 
		: ModeSelectorMenu(startPoint, filePath, visible,
		multimap<int, shared_ptr<drawing::TouchShape>>(), composingMenus, touchEventHandler) {}

	ModeSelectorMenu::ModeSelectorMenu(vec2 startPoint, string filePath, bool visible,
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
		multimap<int, shared_ptr<Menu>> composingMenus, 
		function<void(vec2 point, Menu *self)> touchEventHandler) 
		: Menu(startPoint, width, height, visible, composingShapes, composingMenus, touchEventHandler), 
		filePath(filePath)
	{
		populateShapes();
	}

	void ModeSelectorMenu::populateShapes()
	{
		auto startX = startPoint.x;
		auto startY = startPoint.y;
		auto endX = startX + width;
		auto endY = startY + height;
		auto imageStart = vec2(startX + imageOffsetX, startY + imageOffsetY);

		AddShape(0, shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(startX, startY, endX, endY, purple, 0, true)));
		AddShape(1, shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(startX, startY, endX, endY, grey, lineThickness, false)));
		if(filePath != "")
		{
			AddShape(2, shared_ptr<drawing::TouchImage>(new drawing::TouchImage(imageStart, imageWidth, imageHeight, filePath)));
		}
	}
	
	//void ModeSelectorMenu::OnTouch(vec2 point)
	//{
	//	if(touchEventHandler != nullptr && visible && boundingRect.Contains(point))
	//	{
	//		touchEventHandler(point, this);
	//	}
	//	MenuGroup::OnTouch(point);
	//}


}}
