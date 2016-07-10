#pragma once
#include "TouchShape.h"
#include "IDrawable.h"
#include "Rect.h"

using namespace std;

namespace touchpoints { namespace ui
{
	class Menu : public drawing::IDrawable
	{
	public:
		Menu();
		Menu(vec2 startPoint, int width, int height, bool visible);
		Menu(vec2 startPoint, int width, int height, bool visible, multimap<int, shared_ptr<drawing::TouchShape>> composingShapes);
		virtual void SetWidth(int width) {};
		virtual void SetHeight(int height) {};
		virtual void SetDimensions(int width, int height) {};
		inline void ToggleVisiblibility() { visible = !visible; }
		inline bool IsVisible() const { return visible; }
		void Draw() override;
		virtual void Update() {};
		virtual void OnTouch(vec2 point) {};
	protected:
		vec2 startPoint;
		int width;
		int height;
		bool visible;
		math::Rect boundingRect;
		//collection of shapes that compose this menu along with their corresponding z-index
		//shapes with low z-index get drawn first, high z-index shapes get drawn after
		multimap<int, shared_ptr<drawing::TouchShape>> composingShapes;
	private:
		void initilizeBoundingRect();
	};
}}
