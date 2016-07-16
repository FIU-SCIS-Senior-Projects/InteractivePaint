#pragma once
#include "TouchShape.h"
#include "Rect.h"
#include "IDrawable.h"


using namespace std;

namespace touchpoints { namespace ui
{
	class Menu : public drawing::IDrawable
	{
	public:
		Menu();
		Menu(vec2 startPoint, int width, int height, bool visible,
			function<void(vec2 point, Menu* self)> touchEventHandler);
		Menu(vec2 startPoint, int width, int height, bool visible, 
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
			function<void(vec2 point, Menu* self)> touchEventHandler);
		Menu(vec2 startPoint, int width, int height, bool visible,
			multimap<int, shared_ptr<Menu>> composingMenus,
			function<void(vec2 point, Menu* self)> touchEventHandler);
		Menu(vec2 startPoint, int width, int height, bool visible,
			multimap<int, shared_ptr<drawing::TouchShape>> composingShapes,
			multimap<int, shared_ptr<Menu>> composingMenus,
			function<void(vec2 point, Menu* self)> touchEventHandler);
		virtual void SetWidth(int width) {};
		virtual void SetHeight(int height) {};
		virtual void SetDimensions(int width, int height) {};
		void Draw() override;
		void AddShape(int zIndex, shared_ptr<drawing::TouchShape> shape);
		void AddMenu(int zIndex, shared_ptr<Menu> menu);
		inline void MakeVisible() { visible = true; }
		inline void MakeInvisible() { visible = false; }
		inline void ToggleVisiblibility() { visible = !visible; }
		inline bool IsVisible() const { return visible; }
		void ToggleContainingMenusVisibility();
		virtual void Update() {};
		virtual void OnTouch(vec2 point);
		static const ColorA grey;
		static const ColorA purple;
		static const ColorA white;
		static const int defaultBorderThickness = 5;
		static const int defaultLineThickness = 1;
		static const int defaultWidth = 60;
		static const int defaultHeight = 60;
		static const int defaultImageWidth = 50;
		static const int defaultImageHeight = 50;
		static const int defaultImageOffsetX = 5;
		static const int defaultImageOffsetY = 5;
		static const function<void(vec2 point, Menu *self)> defaultDropdownCallback;
	protected:
		bool visible;
		vec2 startPoint;
		int width;
		int height;
		math::Rect boundingRect;
		//collection of drawables that that compose this menu along with their corresponding z-index
		//shapes with low z-index get drawn first, high z-index shapes get drawn after
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes;
		multimap<int, shared_ptr<Menu>> composingMenus;
		function<void(vec2 point, Menu* self)> touchEventHandler;
	private:
		void initilizeBoundingRect();
	};
}}
