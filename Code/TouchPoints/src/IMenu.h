#pragma once
#include "IDrawable.h"
#include <glm/detail/type_vec2.hpp>

namespace touchpoints { namespace ui
{
	class IMenu : public drawing::IDrawable
	{
	public:
		inline void MakeVisible() { visible = true; }
		inline void MakeInvisible() { visible = false; }
		inline void ToggleVisiblibility() { visible = !visible; }
		inline bool IsVisible() const { return visible; }
		void Draw() override = 0;
		virtual void Update() {};
		virtual void OnTouch(glm::vec2 point) {};
	protected:
		bool visible;
	};
}}
