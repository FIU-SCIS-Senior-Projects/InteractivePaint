#pragma once
#include "Layer.h"
#include "Menu.h"

namespace touchpoints { namespace ui
{
	class MenuLayer : private drawing::Layer
	{
	public:
		MenuLayer();
		MenuLayer(int windowWidth, int windowHeight);
		void OnTouch(vec2 point) const;
		void AddMenu(shared_ptr<Menu> menu);
		void SetWindowWidth(int width);
		void SetWindowHeight(int height);
		void SetWindowDimensions(int width, int height);
		void Draw();
		void Undo();
		float GetAlpha() const;
		void SetAlpha(float value);
		gl::Texture2dRef GetFrameBufferTexture() const;
	private:
		vector<shared_ptr<Menu>> drawablesStack;
	};
}}
