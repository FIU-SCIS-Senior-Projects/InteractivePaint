#pragma once
#include "TouchShape.h"

using namespace std;

namespace touchpoints { namespace ui
{
	class Menu
	{
	public:
		Menu();
		Menu(int width, int height);
		Menu(int width, int height, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		virtual void SetWidth(int width) {};
		virtual void SetHeight(int height) {};
		virtual void SetDimensions(int width, int height) {};
		void Draw();
		virtual void Update() = 0;
		virtual void OnTouch() {};
	private:
		int width;
		int height;
		//collection of shapes that compose this menu along with their corresponding z-index
		//shapes with low z-index get drawn first, high z-index shapes get drawn after
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes;
	};
}}
