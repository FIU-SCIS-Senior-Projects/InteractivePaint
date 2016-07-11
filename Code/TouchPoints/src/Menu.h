#pragma once
#include "TouchShape.h"
#include "Rect.h"
#include "IMenu.h"

using namespace std;

namespace touchpoints { namespace ui
{
	class Menu : public IMenu
	{
	public:
		Menu();
		Menu(vec2 startPoint, int width, int height, bool visible);
		Menu(vec2 startPoint, int width, int height, bool visible, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		virtual void SetWidth(int width) {};
		virtual void SetHeight(int height) {};
		virtual void SetDimensions(int width, int height) {};
		void Draw() override;
		void AddShape(int zIndex, shared_ptr<drawing::TouchShape> shape);
	protected:
		vec2 startPoint;
		int width;
		int height;
		math::Rect boundingRect;
		//collection of drawables that that compose this menu along with their corresponding z-index
		//shapes with low z-index get drawn first, high z-index shapes get drawn after
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes;
	private:
		void initilizeBoundingRect();
	};
}}
